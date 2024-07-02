#pragma once

#include "UObject/Object.h"
#include "ADisplayMethod.h"
#include "DisplayMethodNames.h"
#include "ScaleConfig.h"
#include "DMDensityFish.generated.h"

UCLASS(BlueprintType)
class UDMDensityFish : public UADisplayMethod
{
	GENERATED_BODY()

public:
    explicit UDMDensityFish()
        : UADisplayMethod()
    {
    	method = DISPLAY_METHOD_DENSITY_FISH;
    }

	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Density fish")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		UScaleConfig* scale;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Density fish")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString model;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Density fish")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		float unit_mass;
};
