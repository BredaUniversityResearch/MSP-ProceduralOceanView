// Copyright notice

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PerlinNoiseGenerator.generated.h"

UCLASS(BlueprintType)
class MSP_PROCEDURALGEN_API UPerlinNoiseGenerator : public UObject
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, Category = "Noise|Perlin")
        static float Perlin2D(float X, float Y, int32 Seed, int32 Octaves);

    UFUNCTION(BlueprintCallable, Category = "Noise|Perlin")
        static float Perlin2DShaped(float X, float Y, int32 Seed, float Scale, float Cover, float Contrast, int32 Octaves);
};
