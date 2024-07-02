#pragma once

#include "UObject/Object.h"
#include "ADisplayMethod.h"
#include "DisplayMethodNames.h"
#include "ScaleConfig.h"
#include "DMBathymetry.generated.h"

UCLASS(BlueprintType)
// ReSharper disable CppInconsistentNaming, need to map with the json
// ReSharper disable once IdentifierTypo
class UDMBathymetry : public UADisplayMethod
{
    GENERATED_BODY()

public:
    explicit UDMBathymetry()
        : UADisplayMethod()
    {
        method = DISPLAY_METHOD_BATHYMETRY;
    }

    // @note(MH) : since 2023-11-16, we use a custom raster format for Bathymetry that just holds the depth in cm, so there is no scaling involved
    //  for more info check https://confluence.cradle.buas.nl/display/3OV/North+Sea+Bathymetry
    // UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Bathymetry")
    //     UScaleConfig* scale;
};
