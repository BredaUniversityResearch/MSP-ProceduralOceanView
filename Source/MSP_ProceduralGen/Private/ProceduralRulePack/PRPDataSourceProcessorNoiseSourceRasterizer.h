#pragma once

#include "DataPool/DataPool.h"
#include "ProceduralRulePack/ProceduralRulePackDataSourceProcessor.h"
#include "PRPDataSourceProcessorNoiseSourceRasterizer.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UPRPDataSourceProcessorNoiseSourceRasterizer: public UProceduralRulePackDataSourceProcessor
{
	GENERATED_BODY()

	static const FString NoiseR1IntensityMetaDataKey; //Known sound intensity at R1 distance, in dB
	static const FString NoiseR1DistanceMetaDataKey;  //Distance of R1 from the source in cm.
	static constexpr double NoiseFloorOcean = 50.0; //In dB, https://www.nature.com/articles/srep00437, Ocean at 52dB for natural/biological noise

public:
	virtual void RegisterWithDataPool_Implementation(UDataPool* DataPool) override;

	UFUNCTION()
	void OnDataSourceChanged(const FName& DataSourceName);

private:
	static void RasterizeNoise(const FDataPoolPoints& DataPoolPoints, UDataPoolRaster* DataPoolRaster);

	UPROPERTY(EditAnywhere)
	int OutputLayerResolutionXY = 1024;

	FOnDataSourceDataUpdatedMulticastDelegate::FDelegate m_dataPoolChangedDelegate;
};