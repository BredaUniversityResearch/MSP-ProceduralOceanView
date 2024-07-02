#pragma once

#include "UObject/Object.h"
#include "ScaleConfig.generated.h"

UCLASS(BlueprintType)
// ReSharper disable CppInconsistentNaming, need to map with the json
class UScaleConfig : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(SkipSerialization, BlueprintReadOnly, Category = "Config|Data model|Layer|Display method")
		float min_value;
	UPROPERTY(SkipSerialization, BlueprintReadOnly, Category = "Config|Data model|Layer|Display method")
		float max_value;
	UPROPERTY(SkipSerialization, BlueprintReadOnly, Category = "Config|Data model|Layer|Display method")
		FString interpolation; /*  Lin, Log, Quad */

	UFUNCTION(BlueprintCallable, Category = "Config|Data model|Layer|Display method", DisplayName="Get the scale minimum value in Unreal world scale")
		float GetWorldScaleMinValue() const;
	UFUNCTION(BlueprintCallable, Category = "Config|Data model|Layer|Display method", DisplayName="Get the scale maximum value in Unreal world scale")
		float GetWorldScaleMaxValue() const;
};
