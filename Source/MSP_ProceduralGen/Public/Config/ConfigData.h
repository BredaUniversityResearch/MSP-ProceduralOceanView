#pragma once

#include "UObject/Object.h"
#include "RasterLayer.h"
#include "VectorLayer.h"
#include "ConfigData.generated.h"

UCLASS(BlueprintType)
// ReSharper disable CppInconsistentNaming, need to map with the json
class UConfigData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model")
		TArray<float> coordinate0;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model")
		TArray<float> coordinate1;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model")
		FString projection;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model")
		TArray<URasterLayer*> raster_layers;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model")
		TArray<UVectorLayer*> vector_layers;

	UFUNCTION(BlueprintCallable, Category = "Config|Data model", DisplayName="Get coordinate0 as world Vector2D")
		FVector2D GetCoordinate0AsWorldVector2D() const;
	UFUNCTION(BlueprintCallable, Category = "Config|Data model", DisplayName="Get coordinate1 as world Vector2D")
		FVector2D GetCoordinate1AsWorldVector2D() const;
	// THIS IS THE WORLD OFFSET VARIABLE FOR THE AREA MESHES
	UFUNCTION(BlueprintCallable, Category = "Config|Data model", DisplayName="Get the world offset")
		FVector2D GetWorldOffset() const;
	UFUNCTION(BlueprintCallable, Category = "Config|Data model", DisplayName="Get the offset")
		FVector2D GetOffset() const;
};
