#include "DataPool/DataPoolRaster.h"

#include "GeoTiffFileData.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Config/RasterLayer.h"
#include "Misc/FileHelper.h"
#include "MSP_ProceduralGen/MSP_ProceduralGen.h"

const FRasterPixelChannelFormat FRasterPixelChannelFormat::Gray8 = FRasterPixelChannelFormat(1, 8, ERGBFormat::Gray);
const FRasterPixelChannelFormat FRasterPixelChannelFormat::Gray16 = FRasterPixelChannelFormat(1, 16, ERGBFormat::Gray);

UDataPoolRasterReadOnly::UDataPoolRasterReadOnly(const FObjectInitializer&)
{
}

double FRasterPixelData::Sample(FIntPoint RasterPosition) const
{
	if (RasterPixelWidth == 0 || RasterPixelHeight == 0)
	{
		UE_LOG(LogMspProceduralGen, Error, TEXT("Sampling empty raster layer"));
		return 0.0;
	}

	if (RasterPosition.X < 0 || RasterPosition.X >= RasterPixelWidth || RasterPosition.Y < 0 || RasterPosition.Y >= RasterPixelHeight)
	{
		//UE_LOG(LogMspProceduralGen, Error, TEXT("Sampling raster layer out of bounds. (X: %i, Y: %i), Size: (X: %i, Y: %i)"),
		//	RasterPosition.X, RasterPosition.Y, RasterPixelWidth, RasterPixelHeight);

		RasterPosition.X = FMath::Clamp(RasterPosition.X, 0, RasterPixelWidth - 1);
		RasterPosition.Y = FMath::Clamp(RasterPosition.Y, 0, RasterPixelHeight - 1);
	}

	const uint8* pixelDataStart = RawData.GetData() + ((RasterPosition.X + (RasterPosition.Y * RasterPixelWidth)) * RawDataChannelFormat.BytesPerPixel);

	uint32 sampleMaxValue = 1;
	uint32 result = 0;
	switch (RawDataChannelFormat.BytesPerPixel)
	{
	case 1:
		result = *pixelDataStart;
		sampleMaxValue = TNumericLimits<uint8>::Max();
		break;
	case 2:
		result = *reinterpret_cast<const uint16*>(pixelDataStart);
		sampleMaxValue = TNumericLimits<uint16>::Max();
		break;
	case 4:
		result = *reinterpret_cast<const uint32*>(pixelDataStart);
		sampleMaxValue = TNumericLimits<uint32>::Max();
		break;
	default:
		checkNoEntry();
		break;
	}

	return static_cast<double>(result) / static_cast<double>(sampleMaxValue);
}

double FRasterPixelData::SampleSubPixel(FVector2D Vector2) const
{
	int baseX = static_cast<int>(FMath::Floor(Vector2.X));
	double fracX = Vector2.X - static_cast<double>(baseX);
	int baseY = static_cast<int>(FMath::Floor(Vector2.Y));
	double fracY = Vector2.Y - static_cast<double>(baseY);

	double s00 = Sample(FIntPoint(baseX, baseY));
	double s01 = Sample(FIntPoint(baseX, baseY + 1));
	double s10 = Sample(FIntPoint(baseX + 1, baseY));
	double s11 = Sample(FIntPoint(baseX + 1, baseY + 1));

	double sampleTop = FMath::Lerp(s00, s10, fracX);
	double sampleBot = FMath::Lerp(s01, s11, fracX);

	double sample = FMath::Lerp(sampleTop, sampleBot, fracY);

	return sample;
}

void FRasterPixelData::WriteNormalizedValueAt(FIntPoint RasterPosition, double Value)
{
	if (RasterPosition.X < 0 || RasterPosition.X >= RasterPixelWidth || RasterPosition.Y < 0 || RasterPosition.Y >= RasterPixelHeight)
	{
		UE_LOG(LogMspProceduralGen, Error, TEXT("Writing to raster layer out of bounds. (X: %i, Y: %i), Size: (X: %i, Y: %i)"),
			RasterPosition.X, RasterPosition.Y, RasterPixelWidth, RasterPixelHeight);
		return;
	}

	if (Value < 0 || Value > 1)
	{
		UE_LOG(LogMspProceduralGen, Error, TEXT("Writing to raster layer with non normalized value, Value: %f. (X: %i, Y: %i), Size: (X: %i, Y: %i)"),
			Value, RasterPosition.X, RasterPosition.Y, RasterPixelWidth, RasterPixelHeight);
		return;
	}

	uint8* pixelDataStart = RawData.GetData() + ((RasterPosition.X + (RasterPosition.Y * RasterPixelWidth)) * RawDataChannelFormat.BytesPerPixel);

	switch (RawDataChannelFormat.BytesPerPixel)
	{
	case 1:
		*pixelDataStart = static_cast<uint8>(Value * TNumericLimits<uint8>::Max());
		break;
	case 2:
		*reinterpret_cast<uint16*>(pixelDataStart) = static_cast<uint16>(Value * TNumericLimits<uint16>::Max());
		break;
	case 4:
		*reinterpret_cast<uint32*>(pixelDataStart) = static_cast<uint32>(Value * TNumericLimits<uint32>::Max());
		break;
	default:
		checkNoEntry();
		break;
	}
}

void FRasterPixelData::SetAllRasterValuesToZero()
{
	FMemory::Memset(RawData.GetData(), 0, RawData.Num());
}

FRasterPixelData FRasterPixelData::CopySubRegion(FIntPoint RasterMin, FIntPoint RasterMax)
{
	check(RasterMin.X >= 0 && RasterMin.X < RasterMax.X && RasterMax.X < RasterPixelWidth &&
		RasterMin.Y >= 0 && RasterMin.Y < RasterMax.Y && RasterMax.Y < RasterPixelHeight);

	int lineWidthBytes = (RasterMax.X - RasterMin.X) * RawDataChannelFormat.BytesPerPixel;
	int linesToCopy = RasterMax.Y - RasterMin.Y;

	FRasterPixelData result;
	result.RasterPixelWidth = RasterMax.X - RasterMin.X;
	result.RasterPixelHeight = RasterMax.Y - RasterMin.Y;
	result.RawDataChannelFormat = RawDataChannelFormat;
	result.RawData.AddUninitialized(result.RasterPixelWidth * result.RasterPixelHeight * result.RawDataChannelFormat.BytesPerPixel);

	const uint8* sourceImageData = RawData.GetData() + ((RasterMin.X + (RasterMin.Y * RasterPixelWidth)) * RawDataChannelFormat.BytesPerPixel);
	uint8* destinationImageData = result.RawData.GetData();
	int sourceImagePitchBytes = RasterPixelWidth * RawDataChannelFormat.BytesPerPixel;
	int destinationImagePitchBytes = result.RasterPixelWidth * result.RawDataChannelFormat.BytesPerPixel;
	for (int y = 0; y < linesToCopy; ++y)
	{
		FMemory::Memcpy(destinationImageData, sourceImageData, lineWidthBytes);
		sourceImageData += sourceImagePitchBytes;
		destinationImageData += destinationImagePitchBytes;
	}
	return result;
}

FRasterPixelData FRasterPixelData::CreateForFormat(int32 PixelWidth, int32 PixelHeight, FRasterPixelChannelFormat Format)
{
	check(PixelWidth > 0 && PixelHeight > 0);
	FRasterPixelData result;
	result.RasterPixelWidth = PixelWidth;
	result.RasterPixelHeight = PixelHeight;
	result.RawData.AddZeroed(PixelWidth * PixelHeight * Format.BytesPerPixel);
	result.RawDataChannelFormat = Format;
	return result;
}

FVector2D FRasterProjection::WorldToRasterSubPixel(FVector2D WorldXY) const
{
	FVector2D offset = WorldXY - OriginWorldSpaceUnrealUnits;
//	if (YAxisFlipped)
//	{
//		offset.Y = -offset.Y;
//	}
	offset = offset / PixelPitchInUnrealUnits;
	FVector2D rasterSpacePosition = offset + OriginRasterSpace;
	return rasterSpacePosition;
}

FIntPoint FRasterProjection::WorldToRaster(FVector2D WorldXY) const
{
	FVector2D subPixel = WorldToRasterSubPixel(WorldXY);
	FIntPoint rasterPoint = FIntPoint(static_cast<int32>(FMath::RoundToInt(subPixel.X)), static_cast<int32>(FMath::RoundToInt(subPixel.Y)));
	return rasterPoint;
}

FVector2D FRasterProjection::RasterSubPixelToWorld(FVector2D RasterXY) const
{
	FVector2D offset = RasterXY - OriginRasterSpace;
	offset = offset * PixelPitchInUnrealUnits;
//	if (YAxisFlipped)
//	{
//		offset.Y = -offset.Y;
//	}
	offset = OriginWorldSpaceUnrealUnits + offset;
	return offset;
}

FRasterProjection FRasterProjection::CreateForBounds(FVector2D BoundsMin, FVector2D BoundsMax, const FRasterPixelData& PixelData)
{
	FRasterProjection projection = {};
	projection.OriginRasterSpace = FVector2D(0, 0);
	//projection.OriginRasterSpace = FVector2D(0, PixelData.RasterPixelHeight);
	projection.OriginWorldSpaceUnrealUnits = BoundsMin;

	const FVector2D rasterWorldSize = BoundsMax - BoundsMin;
	FVector2D rasterPixelSize = rasterWorldSize / FVector2D(PixelData.RasterPixelWidth, PixelData.RasterPixelHeight);

	//rasterPixelSize.Y = -rasterPixelSize.Y;

	projection.PixelPitchInUnrealUnits = rasterPixelSize;
	return projection;
}

double UDataPoolRaster::FRasterLinearGroupedValueMapping::RasterToMappedValue(double RasterValue) const
{
    // Ensure RasterValue is within [0, 1]
    RasterValue = FMath::Clamp(RasterValue, 0.0, 1.0);

    // Find the group that RasterValue belongs to
    int32 GroupIndex = Items.Num() - 1;
    for (; GroupIndex >= 0; --GroupIndex)
    {
        if (RasterValue >= Items[GroupIndex].InputValueNormalised)
            break;
    }

    // If GroupIndex is less than 0, RasterValue is less than the first group's InputValueNormalised
    if (GroupIndex < 0)
        return Items[0].OutputValueRangeMin;

    // If GroupIndex is the last index, return the last group's OutputValueRangeMin
    if (GroupIndex == Items.Num() - 1)
        return Items[GroupIndex].OutputValueRangeMin;

    // Linear interpolation between the two groups
    const double& x0 = Items[GroupIndex].InputValueNormalised;
    const double& x1 = Items[GroupIndex + 1].InputValueNormalised;
    const double& y0 = Items[GroupIndex].OutputValueRangeMin;
    const double& y1 = Items[GroupIndex + 1].OutputValueRangeMin;

    // Perform linear interpolation
    return y0 + (RasterValue - x0) * (y1 - y0) / (x1 - x0);
}

double UDataPoolRaster::FRasterLinearGroupedValueMapping::MappedToRasterValue(double PeakValue) const
{
    // Ensure PeakValue is within the range of output values
    PeakValue = FMath::Clamp(PeakValue, Items[0].OutputValueRangeMin, Items.Last().OutputValueRangeMin);

    // Find the group that PeakValue belongs to
    int32 GroupIndex = 0;
    for (; GroupIndex < Items.Num(); ++GroupIndex)
    {
        if (PeakValue <= Items[GroupIndex].OutputValueRangeMin)
            break;
    }

    // If GroupIndex is 0, return the normalized input value of the first group
    if (GroupIndex == 0)
        return Items[0].InputValueNormalised;

    // If GroupIndex is the last index, return the normalized input value of the last group
    if (GroupIndex == Items.Num())
        return 1.0;

    // Linear interpolation between the two groups
    const double& x0 = Items[GroupIndex - 1].OutputValueRangeMin;
    const double& x1 = Items[GroupIndex].OutputValueRangeMin;
    const double& y0 = Items[GroupIndex - 1].InputValueNormalised;
    const double& y1 = Items[GroupIndex].InputValueNormalised;

    // Perform linear interpolation
    return y0 + (PeakValue - x0) * (y1 - y0) / (x1 - x0);
}

double UDataPoolRaster::FRasterLinearValueMapping::RasterToMappedValue(double RasterValue) const
{
	return FMath::Lerp(OutputValueRangeMin, OutputValueRangeMax, RasterValue);
}

double UDataPoolRaster::FRasterLinearValueMapping::MappedToRasterValue(double PeakValue) const
{
	return FMath::GetRangePct(OutputValueRangeMin, OutputValueRangeMax, PeakValue);
}

FRasterTypeMapping::FRasterTypeMapping(const URasterLayer& FromConfig)
{
	int lastMax = -1;

	constexpr double ConfigMappingMaxValue = 255.0;
	for (const FRasterValueTypeMapping& value : FromConfig.mapping)
	{
		verifyf(lastMax <= value.min, TEXT("Config for raster layer specifies overlapping / unsorted range. Type min %i overlaps with last max %i on layer %s"), value.min, lastMax, *FromConfig.name);

		FRasterValueToTypeMapping entry;
		entry.NormalizedMin = FMath::Clamp(static_cast<double>(value.min) / ConfigMappingMaxValue, 0.0, 1.0);
		entry.NormalizedMax = FMath::Clamp(static_cast<double>(value.max) / ConfigMappingMaxValue, 0.0, 1.0);
		entry.TypeIndex = value.type;
		entry.Name = FName(FromConfig.types[value.type]->m_values["name"]);
		Mappings.Emplace(entry);
	}
}

FRasterValueToTypeMapping FRasterTypeMapping::RasterValueToType(double NormalizedRasterValue) const
{
	for (const auto& it : Mappings)
	{
		if (NormalizedRasterValue >= it.NormalizedMin && NormalizedRasterValue < it.NormalizedMax)
		{
			return it;
		}
	}
	return FRasterValueToTypeMapping();
}

int32 FRasterTypeMapping::RasterValueToTypeIndex(double NormalizedRasterValue) const
{
	return RasterValueToType(NormalizedRasterValue).TypeIndex;
}

bool UDataPoolRaster::LoadImageDataFromImageFile(const FString& a_resolvedImageFilePath, FRasterPixelData& a_result, FRasterProjection& a_projection, bool& a_fileContainsProjection, FRasterLinearValueMapping& a_valueMapping)
{
	a_fileContainsProjection = false;

	TArray<uint8> rawFileData;
	if (!FFileHelper::LoadFileToArray(rawFileData, *a_resolvedImageFilePath))
	{
		return false;
	}
	IImageWrapperModule& imageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	EImageFormat imageFormat = imageWrapperModule.DetectImageFormat(rawFileData.GetData(), rawFileData.Num());

	const TSharedPtr<IImageWrapper> imageWrapper = imageWrapperModule.CreateImageWrapper(imageFormat, *a_resolvedImageFilePath);

	a_result.RawDataChannelFormat = FRasterPixelChannelFormat::Gray8;

	if (imageFormat == EImageFormat::TIFF)
	{
		TUniquePtr<const FGeoTiffFileData> geoTiffData = FGeoTiffFileData::LoadFromFileBytes(rawFileData);
		if (geoTiffData != nullptr)
		{
			a_projection.OriginRasterSpace = FVector2D(geoTiffData->ModelTiepointRasterOrigin.X, geoTiffData->ModelTiepointRasterOrigin.Y);
			a_projection.OriginWorldSpaceUnrealUnits = geoTiffData->GetProjectedTiepointAsUnrealUnits();
			a_projection.PixelPitchInUnrealUnits = geoTiffData->GetPixelScaleAsUnrealUnits();
			//a_projection.YAxisFlipped = false;
			a_fileContainsProjection = true;

			if (geoTiffData->TiffSamplesPerPixel != 1)
			{
				UE_LOG(LogMspProceduralGen, Warning, TEXT("Loaded GeoTiff file (%s) with more than one sample per pixel. File has %i samples per pixel"), *a_resolvedImageFilePath, geoTiffData->TiffSamplesPerPixel);
			}
			switch (geoTiffData->TiffBitsPerSample)
			{
			default:
				UE_LOG(LogMspProceduralGen, Warning, TEXT("Loaded GeoTiff file (%s) with an unknown number of bits per sample: %i"), *a_resolvedImageFilePath, geoTiffData->TiffBitsPerSample);
				//Fall-through
			case 8:
				a_result.RawDataChannelFormat = FRasterPixelChannelFormat::Gray8;
				break;
			case 16:
			case 32://Unreal is the bane of my existence, Unreal treats 32-bit integer textures as 16 bit... TiffImageWrapper.cpp line 669
				a_result.RawDataChannelFormat = FRasterPixelChannelFormat::Gray16;
				break;
			}

			//By default assume we map the entire range.
			a_valueMapping.OutputValueRangeMin = 0;
			a_valueMapping.OutputValueRangeMax = (1 << a_result.RawDataChannelFormat.BitsPerChannel);
		}
	}

	if (!(imageWrapper.IsValid() && imageWrapper->SetCompressed(rawFileData.GetData(), rawFileData.Num())))
	{
		UE_LOG(LogMspProceduralGen, Error, TEXT("Failed to load image file at path %s"), *a_resolvedImageFilePath);
		return false;
	}

	if (!imageWrapper->GetRaw(a_result.RawDataChannelFormat.PixelFormat, a_result.RawDataChannelFormat.BitsPerChannel, a_result.RawData))
	{
		return false;
	}
	a_result.RasterPixelWidth = static_cast<int32>(imageWrapper->GetWidth());
	a_result.RasterPixelHeight = static_cast<int32>(imageWrapper->GetHeight());

	return true;
}

UDataPoolRaster* UDataPoolRaster::CreateWithFormatAndBounds(UDataPool* OwningDataPool, FName RasterDataLayerName, int RasterSizePixelsXY, FRasterPixelChannelFormat ChannelFormat, const FBox2D& RasterBoundsWorld)
{
	UDataPoolRaster* raster = NewObject<UDataPoolRaster>(OwningDataPool, MakeUniqueObjectName(OwningDataPool, UDataPoolRaster::StaticClass(), RasterDataLayerName));
	FRasterPixelData defaultPixelData = FRasterPixelData::CreateForFormat(RasterSizePixelsXY, RasterSizePixelsXY, ChannelFormat);
	FRasterProjection rasterProjection = FRasterProjection::CreateForBounds(RasterBoundsWorld.Min, RasterBoundsWorld.Max, defaultPixelData);
	raster->Init(RasterDataLayerName, MoveTemp(defaultPixelData), MoveTemp(rasterProjection), FRasterLinearGroupedValueMapping(FRasterLinearValueMapping()), {});
	return raster;
}

void UDataPoolRaster::Init(FName DataLayerName, FRasterPixelData RasterPixelData, FRasterProjection WorldToRasterProjection, FRasterLinearGroupedValueMapping GroupedValueMapping, FRasterTypeMapping TypeMapping)
{
	m_dataLayerName = DataLayerName;
	m_rasterPixelData = MoveTemp(RasterPixelData);
	m_worldToRasterProjection = MoveTemp(WorldToRasterProjection);
	m_groupedValueMapping = MoveTemp(GroupedValueMapping);
	m_typeMapping = MoveTemp(TypeMapping);
}

double UDataPoolRaster::GetMappedValueAtWorldPosition(FVector2D WorldXY) const
{
	FVector2D rasterPosition = m_worldToRasterProjection.WorldToRasterSubPixel(WorldXY);
	double rasterValue = m_rasterPixelData.SampleSubPixel(rasterPosition);
	double mappedValue = m_groupedValueMapping.RasterToMappedValue(rasterValue);
	return mappedValue;
}

double UDataPoolRaster::GetNormalizedValueAtWorldPosition(FVector2D WorldXY) const
{
	FVector2D rasterPosition = m_worldToRasterProjection.WorldToRasterSubPixel(WorldXY);
	double rasterValue = m_rasterPixelData.SampleSubPixel(rasterPosition);
	return rasterValue;
}

double UDataPoolRaster::GetNormalizedValueAtRasterPosition(FIntPoint RasterXY) const
{
	double rasterValue = m_rasterPixelData.Sample(RasterXY);
	return rasterValue;
}

int32 UDataPoolRaster::GetMappedTypeIndexAtWorldPosition(FVector2D WorldXY) const
{
	FIntPoint rasterPosition = m_worldToRasterProjection.WorldToRaster(WorldXY);
	double rasterValue = m_rasterPixelData.Sample(rasterPosition);
	int32 typeIndex = m_typeMapping.RasterValueToTypeIndex(rasterValue);
	return typeIndex;
}

FRasterValueToTypeMapping UDataPoolRaster::GetMappedTypeAtWorldPosition(FVector2D WorldXY) const
{
	FIntPoint rasterPosition = m_worldToRasterProjection.WorldToRaster(WorldXY);
	double rasterValue = m_rasterPixelData.Sample(rasterPosition);
	return m_typeMapping.RasterValueToType(rasterValue);
}

void UDataPoolRaster::SetValueMapping(const FRasterLinearValueMapping& ValueMapping)
{
	m_groupedValueMapping = FRasterLinearGroupedValueMapping(ValueMapping);
}

void UDataPoolRaster::SetValueMapping(const FRasterLinearGroupedValueMapping& GroupedValueMapping)
{
	m_groupedValueMapping = GroupedValueMapping;
}

FIntPoint UDataPoolRaster::GetRasterSizePixels() const
{
	return FIntPoint(m_rasterPixelData.RasterPixelWidth, m_rasterPixelData.RasterPixelHeight);
}

FBox2D UDataPoolRaster::GetCoveredWorldArea() const
{
	FVector2D rasterSizeWorld = m_worldToRasterProjection.PixelPitchInUnrealUnits * FVector2D(m_rasterPixelData.RasterPixelWidth, m_rasterPixelData.RasterPixelHeight);
	return FBox2D(m_worldToRasterProjection.OriginWorldSpaceUnrealUnits,
		m_worldToRasterProjection.OriginWorldSpaceUnrealUnits + rasterSizeWorld);
}

const FRasterProjection& UDataPoolRaster::GetProjection() const
{
	return m_worldToRasterProjection;
}

UTexture2D* UDataPoolRaster::CreateTextureFromUnscaledRasterData() const
{
	check(m_rasterPixelData.RawDataChannelFormat.ChannelCount == 1);
	EPixelFormat textureFormat;
	switch (m_rasterPixelData.RawDataChannelFormat.BitsPerChannel)
	{
	case 8:
		textureFormat = PF_G8;
		break;
	case 16:
		textureFormat = PF_G16;
		break;
	default:
		textureFormat = PF_G8;
		checkf(false, TEXT("Unknown bit depth to create texture from raster data. Bit depth: %i"), m_rasterPixelData.RawDataChannelFormat.BitsPerChannel);
		break;
	}

	UTexture2D* loadedT2D = UTexture2D::CreateTransient(m_rasterPixelData.RasterPixelWidth, m_rasterPixelData.RasterPixelHeight, textureFormat);
	if (!loadedT2D || !loadedT2D->GetPlatformData())
	{
		return nullptr;
	}
	void* textureData = loadedT2D->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(textureData, m_rasterPixelData.RawData.GetData(), m_rasterPixelData.RawData.Num());
	loadedT2D->GetPlatformData()->Mips[0].BulkData.Unlock();
	loadedT2D->UpdateResource();
	return loadedT2D;
}


UDataPoolRaster* UDataPoolRaster::MakeWritableCopy() const
{
	FName newName = MakeUniqueObjectName(GetOuter(), UDataPoolRaster::StaticClass(), GetFName());
	UDataPoolRaster* writable = NewObject<UDataPoolRaster>(GetOuter(), newName);
	writable->Init(m_dataLayerName, m_rasterPixelData, m_worldToRasterProjection, m_groupedValueMapping, m_typeMapping);

	return writable;
}

UDataPoolRaster* UDataPoolRaster::CreateResizedBlankRasterWithClonedMappings(int RasterSizePixelsXY, const FBox2D& RasterBoundsWorld) const
{
	FName newName = MakeUniqueObjectName(GetOuter(), UDataPoolRaster::StaticClass(), GetFName());
	UDataPoolRaster* writable = NewObject<UDataPoolRaster>(GetOuter(), newName);
	FRasterPixelData defaultPixelData = FRasterPixelData::CreateForFormat(RasterSizePixelsXY, RasterSizePixelsXY, m_rasterPixelData.RawDataChannelFormat);
	FRasterProjection rasterProjection = FRasterProjection::CreateForBounds(RasterBoundsWorld.Min, RasterBoundsWorld.Max, defaultPixelData);
	writable->Init(m_dataLayerName, MoveTemp(defaultPixelData), MoveTemp(rasterProjection), m_groupedValueMapping, m_typeMapping);
	return writable;
}

void UDataPoolRaster::MakeImmutable()
{
	m_isReadOnlyMode = true;
}

void UDataPoolRaster::WriteNormalizedValueAtRasterPoint(FIntPoint RasterPosition, double Value)
{
	if (m_isReadOnlyMode)
	{
		ReportWriteAccessViolation();
		return;
	}

	m_rasterPixelData.WriteNormalizedValueAt(RasterPosition, Value);
}

void UDataPoolRaster::PlotNormalizedValueGaussianDistribution(FVector2D WorldXY, double Radius, double NormalizedPeakValue)
{
	if (m_isReadOnlyMode)
	{
		ReportWriteAccessViolation();
		return;
	}

	FIntPoint centerRasterPoint = m_worldToRasterProjection.WorldToRaster(WorldXY);
	FIntPoint minRasterPoint = m_worldToRasterProjection.WorldToRaster(WorldXY - FVector2D(Radius, Radius));
	FIntPoint rasterHalfExtents = centerRasterPoint - minRasterPoint;

	if (rasterHalfExtents.X == 0)
	{
		rasterHalfExtents.X = 1;
	}
	if (rasterHalfExtents.Y == 0)
	{
		rasterHalfExtents.Y = 1;
	}

	double distanceScaler = 1.0 / FVector2D(rasterHalfExtents.X, rasterHalfExtents.Y).Length();
	for (int y = -rasterHalfExtents.Y; y < rasterHalfExtents.Y; ++y)
	{
		for (int x = -rasterHalfExtents.X; x < rasterHalfExtents.X; ++x)
		{
			FIntPoint rasterPosition = centerRasterPoint + FIntPoint(x, y);
			if(rasterPosition.X < 0 || rasterPosition.Y < 0 || rasterPosition.X >= m_rasterPixelData.RasterPixelWidth || rasterPosition.Y >= m_rasterPixelData.RasterPixelHeight)
			{
				continue;
			}

			double dist = FMath::Sqrt(static_cast<double>((x * x) + (y * y)));
			double scaledDistance = distanceScaler * dist;
			double gaussianMultiplier = FMath::Exp(-(scaledDistance * scaledDistance) * 8.0);
			double existingValue = m_rasterPixelData.Sample(rasterPosition);
			m_rasterPixelData.WriteNormalizedValueAt(rasterPosition, FMath::Min((existingValue + (gaussianMultiplier * NormalizedPeakValue)), 1.0));
		}
	}
}

void UDataPoolRaster::PlotValueGaussianDistribution(FVector2D WorldXY, double Radius, double PeakValue)
{
	double normalizedPeakValue = m_groupedValueMapping.MappedToRasterValue(PeakValue);
	PlotNormalizedValueGaussianDistribution(WorldXY, Radius, normalizedPeakValue);
}

void UDataPoolRaster::SetAllRasterValuesToZero()
{
	m_rasterPixelData.SetAllRasterValuesToZero();
}

void UDataPoolRaster::ReportWriteAccessViolation() const
{
	UE_LOG(LogMspProceduralGen, Error, TEXT("DataPoolRaster \"%s\" is being written to while read only allowed. Blueprint callstack:\n%s"), *GetFName().ToString(), *FFrame::GetScriptCallstack(true));
	GEngine->AddOnScreenDebugMessage(2469889653296ull, 60.0f, FColor::Red, TEXT("DataPoolRaster is attempted to be changed when in read only. Check the log for more details."));
}
