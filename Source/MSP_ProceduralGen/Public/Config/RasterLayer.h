#pragma once

#include "UObject/Object.h"
#include "ALayer.h"
#include "RasterLayer.generated.h"

UENUM()
enum class ERasterValueMappingInterpolation
{
	Lin,
	LinGrouped
};

USTRUCT()
struct FRasterValueScalingGroups
{
	GENERATED_BODY()

	static constexpr double UnsetValue = TNumericLimits<double>::Min();

	UPROPERTY()
	double normalised_input_value = UnsetValue;

	UPROPERTY()
	double min_output_value = UnsetValue;
};

USTRUCT()
struct FRasterValueScalingData
{
	GENERATED_BODY()

	static constexpr double UnsetValue = TNumericLimits<double>::Min();

	UPROPERTY()
	double min_value = UnsetValue;
	UPROPERTY()
	double max_value = UnsetValue;
	UPROPERTY()
	ERasterValueMappingInterpolation interpolation = ERasterValueMappingInterpolation::Lin;
	UPROPERTY()
	TArray<FRasterValueScalingGroups> groups;
};

USTRUCT()
struct FRasterValueTypeMapping
{
	GENERATED_BODY()

	UPROPERTY()
	int32 min = 0.0; //[0-255]
	UPROPERTY()
	int32 max = 0.0; //[0-255]
	UPROPERTY()
	int32 type = -1;
};

UCLASS(BlueprintType)
// ReSharper disable CppInconsistentNaming, need to map with the json
class URasterLayer : public UALayer
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Raster layer", DisplayName="First corner of the area the raster covers")
	TArray<float> coordinate0;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Raster layer", DisplayName="Second corner of the area the raster covers")
	TArray<float> coordinate1;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Raster layer", DisplayName="Link to file or server API")
	FString data;
	UPROPERTY()
	FRasterValueScalingData scale;
	UPROPERTY()
	TArray<FRasterValueTypeMapping> mapping;

	UFUNCTION(BlueprintCallable, Category = "Config|Data model|Layer|Raster layer", DisplayName="Get coordinate0 as world Vector2D")
		FVector2D GetCoordinate0AsWorldVector2D() const;
	UFUNCTION(BlueprintCallable, Category = "Config|Data model|Layer|Raster layer", DisplayName="Get coordinate1 as world Vector2D")
		FVector2D GetCoordinate1AsWorldVector2D() const;
	UFUNCTION(BlueprintCallable, Category = "Config|Data model|Layer|Raster layer", DisplayName="Get the world offset")
		FVector2D GetWorldOffset() const;
	UFUNCTION(BlueprintCallable, Category = "Config|Data model|Layer|Raster layer", DisplayName="Get the offset")
		FVector2D GetOffset() const;
};
