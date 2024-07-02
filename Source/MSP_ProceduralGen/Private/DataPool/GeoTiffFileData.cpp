#include "GeoTiffFileData.h"

#include <tiffio.h>
#include <tiffiop.h>

#include "MSP_ProceduralGen/MSP_ProceduralGen.h"

namespace
{
	struct FTiffMemoryFileDescriptor
	{
		TArrayView<const uint8> FileAsBytes;
		uint64 FilePosition {}; 

		static tmsize_t FileRead(thandle_t ClientData, void* Target, tmsize_t Size)
		{
			FTiffMemoryFileDescriptor* fd = static_cast<FTiffMemoryFileDescriptor*>(ClientData);

			int64 bytesRemaining = fd->FileAsBytes.Num() - fd->FilePosition;
			int64 bytesToRead = FMath::Min(bytesRemaining, Size);

			FMemory::Memcpy(Target, fd->FileAsBytes.GetData() + fd->FilePosition, bytesToRead);
			fd->FilePosition += bytesToRead;

			return bytesToRead;
		}

		static tmsize_t FileWrite(thandle_t ClientData, void* Target, tmsize_t BytesToRead)
		{
			checkNoEntry();
			return -1;
		}

		static toff_t FileSeek(thandle_t ClientData, toff_t Offset, int whence)
		{
			FTiffMemoryFileDescriptor* fd = static_cast<FTiffMemoryFileDescriptor*>(ClientData);

			int64 targetFilePosition;
			switch(whence)
			{
			case SEEK_SET:
				targetFilePosition = Offset;
				break;
			case SEEK_CUR:
				targetFilePosition = fd->FilePosition + Offset;
				break;
			case SEEK_END:
				targetFilePosition = fd->FileAsBytes.Num() + Offset;
				break;
			default:
				targetFilePosition = {};
				checkNoEntry();
			}

			check(targetFilePosition >= 0 && targetFilePosition < fd->FileAsBytes.Num());
			fd->FilePosition = targetFilePosition;

			return fd->FilePosition;
		}

		static int FileClose(thandle_t ClientData)
		{
			return 0;
		}

		static toff_t FileSize(thandle_t ClientData)
		{
			FTiffMemoryFileDescriptor* fd = static_cast<FTiffMemoryFileDescriptor*>(ClientData);
			return fd->FileAsBytes.Num();
		}

		static int FileMap(thandle_t ClientData, void** base, toff_t* size)
		{
			return 0;
		}

		static void FileUnmap(thandle_t ClientData, void* base, toff_t size)
		{
		}
	};

	struct FGeoTiffGeoKeyDirectoryHeader
	{
		short KeyDirectoryVersion;
		short KeyRevision;
		short MinorRevision;
		short NumberOfKeys;
	};

	struct FGeoTiffGeoKeyHeader
	{
		EGeoTiffGeoKey KeyId;	//See http://geotiff.maptools.org/spec/geotiff6.html for details of each KeyId
		uint16 TIFFTagLocation;	//0 for data is contained here, otherwise it's contained in the TIFF Tag with the ValueOffset-nth entry 
		int16 Count;			//Number of values in this key
		int16 ValueOffset; 
	};

	struct FGeoTiffFieldIdentifiers
	{
		static constexpr uint32 GeoKeyDirectoryTag = 34735;
		static constexpr uint32 GeoDoubleParamsTag = 34736;
		static constexpr uint32 GeoAsciiParamsTag = 34737;
		static constexpr uint32 ModelPixelScaleTag = 33550;
		static constexpr uint32 ModelTiepointTag = 33922;
		static constexpr uint32 ModelTransformationTag = 34264;
	};
};

TUniquePtr<const FGeoTiffFileData> FGeoTiffFileData::LoadFromFile(const FString& ResolvedFilePath)
{
	TArray<uint8> fileAsBytes;
	if (!FFileHelper::LoadFileToArray(fileAsBytes, *ResolvedFilePath)) 
	{
		return {nullptr};
	}
	return LoadFromFileBytes(fileAsBytes);

}

TUniquePtr<const FGeoTiffFileData> FGeoTiffFileData::LoadFromFileBytes(TArrayView<const uint8> FileBytes)
{
	FTiffMemoryFileDescriptor fd;
	fd.FileAsBytes = FileBytes;

	TIFF* tiffFile = TIFFClientOpen("MemTiff", "r", &fd, FTiffMemoryFileDescriptor::FileRead, FTiffMemoryFileDescriptor::FileWrite, 
	FTiffMemoryFileDescriptor::FileSeek, FTiffMemoryFileDescriptor::FileClose, FTiffMemoryFileDescriptor::FileSize, 
	FTiffMemoryFileDescriptor::FileMap, FTiffMemoryFileDescriptor::FileUnmap);

	if (tiffFile == nullptr)
	{
		return {nullptr};
	}

	TUniquePtr<FGeoTiffFileData> raster = TUniquePtr<FGeoTiffFileData>(new FGeoTiffFileData());

	TIFFGetField(tiffFile, TIFFTAG_SAMPLESPERPIXEL, &raster->TiffSamplesPerPixel);
	TIFFGetField(tiffFile, TIFFTAG_BITSPERSAMPLE, &raster->TiffBitsPerSample);
	TIFFGetField(tiffFile, TIFFTAG_SAMPLEFORMAT, &raster->TiffSampleFormat);

	short doubleFieldsCount = 0;
	const double* doubleFieldValues{};
	TIFFGetField(tiffFile, FGeoTiffFieldIdentifiers::GeoDoubleParamsTag, &doubleFieldsCount, &doubleFieldValues);

	short charFieldsCount = 0;
	const char* charFieldValues{};
	TIFFGetField(tiffFile, FGeoTiffFieldIdentifiers::GeoAsciiParamsTag, &charFieldsCount, &charFieldValues);

	short tagCount = 0;
	void* tagData{};
	TIFFGetField(tiffFile, FGeoTiffFieldIdentifiers::GeoKeyDirectoryTag, &tagCount, &tagData);

	const FGeoTiffGeoKeyDirectoryHeader* keyDirectoryHeader = static_cast<const FGeoTiffGeoKeyDirectoryHeader*>(tagData);
	for (int keyIndex = 0; keyIndex < keyDirectoryHeader->NumberOfKeys; ++keyIndex)
	{
		const FGeoTiffGeoKeyHeader* keyHeader = reinterpret_cast<const FGeoTiffGeoKeyHeader*>(static_cast<const uint8*>(tagData) + sizeof(FGeoTiffGeoKeyDirectoryHeader) + (sizeof(FGeoTiffGeoKeyHeader) * keyIndex));

		switch (keyHeader->TIFFTagLocation)
		{
		case 0:
			raster->SetGeoTIFFKey(keyHeader->KeyId, TArrayView<const short>(&keyHeader->ValueOffset, 1));
			break;
		case FGeoTiffFieldIdentifiers::GeoDoubleParamsTag:
		{
			TArrayView<const double> values = TArrayView<const double>(doubleFieldValues + keyHeader->ValueOffset, keyHeader->Count);
			raster->SetGeoTIFFKey(keyHeader->KeyId, values);
		}
			break;
		case FGeoTiffFieldIdentifiers::GeoAsciiParamsTag:
		{
			FAnsiStringView string = FAnsiStringView(charFieldValues + keyHeader->ValueOffset, keyHeader->Count);
			raster->SetGeoTIFFKey(keyHeader->KeyId, string);
		}
			break;
		default:
			UE_LOG(LogMspProceduralGen, Display, TEXT("GeoTiff key %i found with unhandled TIFFTagLocation (%i)"), static_cast<int>(keyHeader->KeyId), keyHeader->TIFFTagLocation);
			break;
		}
	}

	short modelCount{};
	double modelTransformationMatrix[16];
	if (TIFFGetField(tiffFile, FGeoTiffFieldIdentifiers::ModelTransformationTag, &modelCount, &modelTransformationMatrix))
	{
		UE_LOG(LogMspProceduralGen, Warning, TEXT("Encountered model transformation via matrix. This is not supported"));
	}
	else
	{
		double* data{};
		TIFFGetField(tiffFile, FGeoTiffFieldIdentifiers::ModelPixelScaleTag, &modelCount, &data);
		check(modelCount == 3);
		FMemory::Memcpy(raster->ModelPixelScale, data, sizeof(double) * modelCount);
		TIFFGetField(tiffFile, FGeoTiffFieldIdentifiers::ModelTiepointTag, &modelCount, &data);
		check(modelCount == 6);
		raster->ModelTiepointRasterOrigin = FVector(data[0], data[1], data[2]);
		raster->ModelTiepointProjectedOrigin = FVector(data[3], data[4], data[5]);
	}

	TIFFClose(tiffFile);
	
	return raster;
}

double FGeoTiffFileData::GetGeoUnitsToUnrealMultiplier() const
{
	double unrealUnitsPerUnit;
	switch (LinearUnits)
	{
	case EGeoTiffUnits::Linear_Meter:
		unrealUnitsPerUnit = 100; 
		break;
	default:
		unrealUnitsPerUnit = 100;
		UE_LOG(LogMspProceduralGen, Error, TEXT("Encounted unimplemented pixel scale: %i for GeoTiff file data. Defaulting to 100 Unreal units per pixel"), LinearUnits);
		break;
	}
	return unrealUnitsPerUnit;
}

FVector2D FGeoTiffFileData::GetPixelScaleAsUnrealUnits() const
{
	double unitMultiplier = GetGeoUnitsToUnrealMultiplier();
	return FVector2D(ModelPixelScale[0], ModelPixelScale[1]) * unitMultiplier;
}

FVector2D FGeoTiffFileData::GetProjectedTiepointAsUnrealUnits() const
{
	double unitMultiplier = GetGeoUnitsToUnrealMultiplier();
	return FVector2D(ModelTiepointProjectedOrigin.X, -ModelTiepointProjectedOrigin.Y) * unitMultiplier;
}

void FGeoTiffFileData::SetGeoTIFFKey(EGeoTiffGeoKey Key, TArrayView<const short> Values)
{
	switch (Key)
	{
	case EGeoTiffGeoKey::GTModelTypeGeoKey:
		ModelType = static_cast<EGeoTiffModelType>(Values[0]);
		break;
	case EGeoTiffGeoKey::GTRasterTypeGeoKey:
		RasterType = static_cast<EGeoTiffRasterType>(Values[0]);
		break;
	case EGeoTiffGeoKey::ProjLinearUnitsGeoKey:
		LinearUnits = static_cast<EGeoTiffUnits>(Values[0]);
		break;
	case EGeoTiffGeoKey::ProjectedCSTypeGeoKey:
		CoordinateSystemType = Values[0];
		break;
	default:
		UE_LOG(LogMspProceduralGen, Display, TEXT("GeoTiff key %i found (Short Value: %i), but was not handled"), static_cast<int>(Key), Values[0]);
		break;
	}
}

void FGeoTiffFileData::SetGeoTIFFKey(EGeoTiffGeoKey Key, TArrayView<const double> Values)
{
	switch(Key)
	{
	case EGeoTiffGeoKey::Invalid:
		break;
	default:
		UE_LOG(LogMspProceduralGen, Display, TEXT("GeoTiff key %i found (Double Value: %f), but was not handled"), static_cast<int>(Key), Values[0]);
		break;
	}
}

void FGeoTiffFileData::SetGeoTIFFKey(EGeoTiffGeoKey Key, FAnsiStringView Value)
{
	switch (Key)
	{
	case EGeoTiffGeoKey::GeogCitationGeoKey:
		CoordinateSystemCitation = FString(Value);
		break;
	default:
		UE_LOG(LogMspProceduralGen, Display, TEXT("GeoTiff key %i found (String Value: %s), but was not handled"), static_cast<int>(Key), *FString(Value));
		break;
	}
}
