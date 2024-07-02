#pragma once
#include "IImageWrapper.h"
#include "ProceduralRulePack/Fish/PRPDataSourceProcessorRasterPerlinNoiseOverlay.h"

#include "DataPoolRaster.generated.h"

class UDataPoolRaster;
class URasterLayer;

USTRUCT(BlueprintType)
struct FRasterValueToTypeMapping
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="Data pool|Raster data source|Data source type")
	double NormalizedMin{ 0.0 };

	UPROPERTY(BlueprintReadOnly, Category="Data pool|Raster data source|Data source type")
	double NormalizedMax{ 1.0 };

	UPROPERTY(BlueprintReadOnly, Category="Data pool|Raster data source|Data source type", Meta = (DisplayName="Type index (-1 = invalid)"))
	int32 TypeIndex = -1;

	UPROPERTY(BlueprintReadOnly, Category="Data pool|Raster data source|Data source type")
	FName Name;
};

struct FRasterPixelChannelFormat
{
	static const FRasterPixelChannelFormat Gray8;
	static const FRasterPixelChannelFormat Gray16;

	constexpr FRasterPixelChannelFormat(int ChannelCount, int BitsPerChannel, ERGBFormat PixelFormat);

	int ChannelCount;
	int BitsPerChannel;
	int BytesPerPixel;
	ERGBFormat PixelFormat;
};

struct FRasterPixelData
{
	TArray<uint8> RawData;
	FRasterPixelChannelFormat RawDataChannelFormat = FRasterPixelChannelFormat::Gray8;
	int32 RasterPixelWidth;
	int32 RasterPixelHeight;

	double Sample(FIntPoint RasterPosition) const;  //[0..1]
	double SampleSubPixel(FVector2D Vector2) const; //[0..1]

	void WriteNormalizedValueAt(FIntPoint RasterPosition, double Value);
	void SetAllRasterValuesToZero();

	FRasterPixelData CopySubRegion(FIntPoint RasterMin, FIntPoint RasterMax);

	static FRasterPixelData CreateForFormat(int32 PixelWidth, int32 PixelHeight, FRasterPixelChannelFormat Format);
};

struct FRasterProjection
{
	FVector2D WorldToRasterSubPixel(FVector2D WorldXY) const;
	FIntPoint WorldToRaster(FVector2D WorldXY) const;
	FVector2D RasterSubPixelToWorld(FVector2D RasterXY) const;

	static FRasterProjection CreateForBounds(FVector2D BoundsMin, FVector2D BoundsMax, const FRasterPixelData& PixelData);

	//bool YAxisFlipped{ false }; //GeoTiff in EPSG:3035 at least flips the Y that top left is [0, Height] - PdG-2024-03-01: Turns out our projection was upside-down
	FVector2D PixelPitchInUnrealUnits{ 100, 100 }; //How many unreal units we have per pixel
	FVector2D OriginRasterSpace{};
	FVector2D OriginWorldSpaceUnrealUnits{};
};

UINTERFACE(NotBlueprintable, BlueprintType, MinimalAPI)
class UDataPoolRasterReadOnly : public UInterface
{
	GENERATED_UINTERFACE_BODY()
public:
};

class IDataPoolRasterReadOnly
{
	GENERATED_IINTERFACE_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Data pool|Raster data source", meta = (ToolTip = "Gets the mapped value that is present at WorldXY on mapped onto the raster"))
		virtual double GetMappedValueAtWorldPosition(FVector2D WorldXY) const = 0;
	UFUNCTION(BlueprintCallable, Category = "Data pool|Raster data source", meta = (ToolTip = "Gets the normalized [0..1] value that is present at WorldXY on mapped onto the raster"))
		virtual double GetNormalizedValueAtWorldPosition(FVector2D WorldXY) const = 0;
	UFUNCTION(BlueprintCallable, Category = "Data pool|Raster data source", meta = (ToolTip = "Gets the normalized [0..1] value that is present at pixel position RasterX"))
		virtual double GetNormalizedValueAtRasterPosition(FIntPoint RasterXY) const = 0;
	UFUNCTION(BlueprintCallable, Category = "Data pool|Raster data source", meta = (ToolTip = "Gets the mapped type at WorldXY. Returns -1 if no type is mapped at the requested location"))
		virtual int32 GetMappedTypeIndexAtWorldPosition(FVector2D WorldXY) const = 0;
	UFUNCTION(BlueprintCallable, Category = "Data pool|Raster data source", meta = (ToolTip = "Gets the mapped type at WorldXY. Its TypeIndex value will be -1 if no type is mapped at the requested location."))
		virtual FRasterValueToTypeMapping GetMappedTypeAtWorldPosition(FVector2D WorldXY) const = 0;

	UFUNCTION(BlueprintCallable, Category = "Data pool|Raster data source", meta = (ToolTip = "Gets the area this raster represents in unreal world space"))
		virtual FBox2D GetCoveredWorldArea() const = 0;
	virtual const FRasterProjection& GetProjection() const = 0;
	virtual FIntPoint GetRasterSizePixels() const = 0;

	UFUNCTION(BlueprintCallable, Category = "Data pool|Raster data source", BlueprintPure = false)
		virtual UTexture2D* CreateTextureFromUnscaledRasterData() const = 0;

	UFUNCTION(BlueprintCallable, Category = "Data pool|Raster data source", BlueprintPure = false)
		virtual UDataPoolRaster* MakeWritableCopy() const = 0;
	UFUNCTION(BlueprintCallable, Category = "Data pool|Raster data source", BlueprintPure = false)
		virtual UDataPoolRaster* CreateResizedBlankRasterWithClonedMappings(int RasterSizePixelsXY, const FBox2D& RasterBoundsWorld) const = 0;
};

struct FRasterTypeMapping
{
	FRasterTypeMapping() = default;
	FRasterTypeMapping(const URasterLayer& FromConfig);

	FRasterValueToTypeMapping RasterValueToType(double NormalizedRasterValue) const;
	int32 RasterValueToTypeIndex(double NormalizedRasterValue) const;

	TArray<FRasterValueToTypeMapping> Mappings = {};
};

UCLASS(BlueprintType)
class UDataPoolRaster : public UObject, public IDataPoolRasterReadOnly
{
	GENERATED_BODY()
public:
	struct FRasterLinearGroupedItem
	{
		FRasterLinearGroupedItem(const double a_inputValueNormalised, const double a_outputValueRangeMin)
			: InputValueNormalised(a_inputValueNormalised),
			  OutputValueRangeMin(a_outputValueRangeMin)
		{
		}

		double InputValueNormalised;
		// note that the "OutputValueRangeMax" needs to be distracted from next group item's OutputValueRangeMin
		double OutputValueRangeMin;
	};

	struct FRasterLinearValueMapping
	{
		double RasterToMappedValue(double RasterValue) const;
		double MappedToRasterValue(double PeakValue) const;

		double OutputValueRangeMin{ 0 };
		double OutputValueRangeMax{ 255.0f };
	};

	struct FRasterLinearGroupedValueMapping
	{
		FRasterLinearGroupedValueMapping() = default;
		explicit FRasterLinearGroupedValueMapping(const FRasterLinearValueMapping& a_linearValueMapping) :
			Items({
				FRasterLinearGroupedItem(0, a_linearValueMapping.OutputValueRangeMin),
				FRasterLinearGroupedItem(1, a_linearValueMapping.OutputValueRangeMax)
			})
		{
		}

		double RasterToMappedValue(double RasterValue) const;
		double MappedToRasterValue(double PeakValue) const;

		TArray<FRasterLinearGroupedItem> Items;
	};

	static bool LoadImageDataFromImageFile(const FString& a_resolvedImageFilePath, FRasterPixelData& a_result, FRasterProjection& a_projection, bool& a_fileContainsProjection, FRasterLinearValueMapping& a_valueMapping);
	static UDataPoolRaster* CreateWithFormatAndBounds(UDataPool* OwningDataPool, FName RasterDataLayerName, int RasterSizePixelsXY, FRasterPixelChannelFormat ChannelFormat, const FBox2D& RasterBoundsWorld);

	void Init(FName DataLayerName, FRasterPixelData RasterPixelData, FRasterProjection WorldToRasterProjection, FRasterLinearGroupedValueMapping GroupedValueMapping, FRasterTypeMapping TypeMapping);

	virtual double GetMappedValueAtWorldPosition(FVector2D WorldXY) const override;
	virtual double GetNormalizedValueAtWorldPosition(FVector2D WorldXY) const override;
	virtual double GetNormalizedValueAtRasterPosition(FIntPoint RasterXY) const override;
	virtual int32 GetMappedTypeIndexAtWorldPosition(FVector2D WorldXY) const override;
	virtual FRasterValueToTypeMapping GetMappedTypeAtWorldPosition(FVector2D WorldXY) const override;
	void SetValueMapping(const FRasterLinearValueMapping& ValueMapping);
	void SetValueMapping(const FRasterLinearGroupedValueMapping& GroupedValueMapping);
	virtual FIntPoint GetRasterSizePixels() const override;

	virtual FBox2D GetCoveredWorldArea() const override;
	virtual const FRasterProjection& GetProjection() const override;

	virtual UTexture2D* CreateTextureFromUnscaledRasterData() const override;

	virtual UDataPoolRaster* MakeWritableCopy() const override;
	virtual UDataPoolRaster* CreateResizedBlankRasterWithClonedMappings(int RasterSizePixelsXY, const FBox2D& RasterBoundsWorld) const override;

	void MakeImmutable();

	void WriteNormalizedValueAtRasterPoint(FIntPoint RasterPosition, double Value);
	void PlotNormalizedValueGaussianDistribution(FVector2D WorldXY, double Radius, double NormalizedPeakValue);
	void PlotValueGaussianDistribution(FVector2D WorldXY, double Radius, double PeakValue);
	void SetAllRasterValuesToZero();

private:
	void ReportWriteAccessViolation() const;

	FName m_dataLayerName; //So we can identify what layer we are sampling when debugging.
	bool m_isReadOnlyMode{ false };
	FRasterPixelData m_rasterPixelData{};
	FRasterProjection m_worldToRasterProjection{};
	// default is a linear value mapping, but converted to a grouped one
	FRasterLinearGroupedValueMapping m_groupedValueMapping{FRasterLinearValueMapping()};
	FRasterTypeMapping m_typeMapping{};
};

constexpr FRasterPixelChannelFormat::FRasterPixelChannelFormat(int ChannelCount, int BitsPerChannel, ERGBFormat PixelFormat)
	: ChannelCount(ChannelCount)
	, BitsPerChannel(BitsPerChannel)
	, BytesPerPixel((ChannelCount* BitsPerChannel) / 8)
	, PixelFormat(PixelFormat)
{
}
