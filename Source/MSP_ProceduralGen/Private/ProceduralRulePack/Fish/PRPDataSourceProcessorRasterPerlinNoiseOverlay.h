// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DataPool/DataPool.h"
#include "ProceduralRulePack/ProceduralRulePackDataSourceProcessor.h"
#include "PRPDataSourceProcessorRasterPerlinNoiseOverlay.generated.h"

class UDataPoolRaster;

UCLASS(Blueprintable, Abstract)
class UPRPDataSourceProcessorRasterPerlinNoiseOverlay : public UProceduralRulePackDataSourceProcessor
{
	GENERATED_BODY()

	static constexpr double NoiseOffsetRange = 16384;

public: 
	void RegisterWithDataPool_Implementation(UDataPool* DataPool) override;

	UFUNCTION()
	void OnDataSourceChanged(const FName& DataSourceName);

private:
	void RecalculateOutputLayer(const IDataPoolRasterReadOnly* SourceLayer, UDataPoolRaster& TargetRaster) const;

	UPROPERTY(EditAnywhere) //Read and write to this layer.
	FName DataPoolTargetRasterLayer;
	bool HasProcessed = false; // Ensure we only process once.

	UPROPERTY(EditAnywhere)
	double NoiseStrength = 0.1; //Amount of normalised intensity to add or substract
	UPROPERTY(EditAnywhere)
	int NoiseOctaves = 1;
	UPROPERTY(EditAnywhere)
	double NoiseFrequency = 5.0; //Perlin noise size, input range is normalised to 0..1 range and then multiplied by this.
	UPROPERTY(EditAnywhere)
	double NoisePersistence = 0.5; //Roughness. Amplitude multiplier between successive ocatves.
	UPROPERTY(EditAnywhere)
	double NoiseLacunarity = 2.0; //Frequency multiplier between successive octaves

	//FOnDataSourceDataUpdatedMulticastDelegate::FDelegate DataPoolChangedHandler;
};
