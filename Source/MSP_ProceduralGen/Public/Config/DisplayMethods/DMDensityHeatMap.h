#pragma once

#include "UObject/Object.h"
#include "ADisplayMethod.h"
#include "DisplayMethodNames.h"
#include "ScaleConfig.h"
#include "DMDensityHeatMap.generated.h"

UCLASS(BlueprintType)
class UDMDensityHeatMap : public UADisplayMethod
{
	GENERATED_BODY()

public:
    explicit UDMDensityHeatMap()
        : UADisplayMethod()
    {
    	method = DISPLAY_METHOD_DENSITY_HEAT_MAP;
    }

	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Density heatmap")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		UScaleConfig* scale;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Density heatmap")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString colour_set;
};
