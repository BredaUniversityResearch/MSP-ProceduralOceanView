// Fill out your copyright notice in the Description page of Project Settings.
#include "ProceduralRulePack/Fish/PRPDataSourceProcessorRasterPerlinNoiseOverlay.h"

#include "DataPool/DataPool.h"
#include "DataPool/DataPoolRaster.h"

#include "DataPool/DataPoolDataSourceNames.h"

void UPRPDataSourceProcessorRasterPerlinNoiseOverlay::RegisterWithDataPool_Implementation(UDataPool* DataPool)
{
    Super::RegisterWithDataPool_Implementation(DataPool);

    //bool regSuccessNoise = DataPool->TryRegisterDataUpdatedListener(DataPoolTargetRasterLayer, DataPoolChangedHandler);
    //check(regSuccessNoise);

    OnDataSourceChanged(DataPoolTargetRasterLayer);
}

void UPRPDataSourceProcessorRasterPerlinNoiseOverlay::OnDataSourceChanged(const FName& dataSourceName)
{
    IDataPoolRasterReadOnly* sourceData = nullptr;
    if (TargetDataPool->TryGetData(DataPoolTargetRasterLayer, sourceData))
    {
        FBox2D rasterBounds = sourceData->GetCoveredWorldArea();

        UDataPoolRaster* raster = sourceData->CreateResizedBlankRasterWithClonedMappings(1024, rasterBounds);

        RecalculateOutputLayer(sourceData, *raster);

        TargetDataPool->TrySetData(DataPoolTargetRasterLayer, TStrongObjectPtr(raster));
    }
}

void UPRPDataSourceProcessorRasterPerlinNoiseOverlay::RecalculateOutputLayer(const IDataPoolRasterReadOnly* SourceLayer, UDataPoolRaster& TargetRaster) const
{
    TargetRaster.SetAllRasterValuesToZero();
    FIntPoint outputSize = TargetRaster.GetRasterSizePixels();

    FVector2D noiseOffset = SourceLayer->GetCoveredWorldArea().Min + FVector2D(DataPoolTargetRasterLayer.GetComparisonIndex().ToUnstableInt(), 0);
    noiseOffset.X = FMath::Fmod(noiseOffset.X, NoiseOffsetRange);
    noiseOffset.Y = FMath::Fmod(noiseOffset.Y, NoiseOffsetRange);

    for (int y = 0; y < outputSize.Y; ++y)
    {
        for (int x = 0; x < outputSize.X; ++x)
        {
            FVector2D worldPos = TargetRaster.GetProjection().RasterSubPixelToWorld(FVector2D(x, y));
            double sourceValue = SourceLayer->GetNormalizedValueAtWorldPosition(worldPos);

            FVector2D localNoisePosition = (FVector2D(x, y) / outputSize) * NoiseFrequency;

            double noiseValue = 0.0;
            double amplitude = 1.0f;
        	for (int i = 0; i < NoiseOctaves; ++i)
        	{
        		noiseValue += FMath::PerlinNoise2D(localNoisePosition + noiseOffset) * amplitude; // [-1, 1]
                amplitude *= NoisePersistence;
                localNoisePosition *= NoiseLacunarity;
            }

            double targetValue = sourceValue + (NoiseStrength * noiseValue);

            TargetRaster.WriteNormalizedValueAtRasterPoint(FIntPoint(x, y), FMath::Clamp(targetValue, 0.0, 1.0));
        }
    }
}
