#pragma once

#include "DataPool/DataPool.h"
#include "ProceduralRulePack/ProceduralRulePackDataSourceProcessor.h"
#include "PRPDataSourceProcessorRasterHotspotLocator.generated.h"

class UDataPoolRaster;

UCLASS(Blueprintable, Abstract)
class UPRPDataSourceProcessorRasterHotspotLocator : public UProceduralRulePackDataSourceProcessor
{
	GENERATED_BODY()

public: 
	void RegisterWithDataPool_Implementation(UDataPool* DataPool) override;

	UFUNCTION()
	void OnDataSourceChanged(const FName& DataSourceName);

private:
	void RecalculateOutputLayer(const FBox2D& SimulationBounds, const IDataPoolRasterReadOnly& SourceLayer, FDataPoolPoints& TargetPoints) const;

	void SampleRasterCell(const IDataPoolRasterReadOnly& SourceLayer, const FIntRect& RasterBounds, double& MaxValue, FIntPoint& MaxValuePixelLocation) const;

	UPROPERTY(EditAnywhere) //Read from this layer.
	FName DataPoolSourceRasterLayer;
	UPROPERTY(EditAnywhere) //Write to this layer.
	FName DataPoolTargetPointLayer;

	UPROPERTY(EditAnywhere)
	int GridAxisDivisionCount = 16;
	UPROPERTY(EditAnywhere)
	int HotSpotsToTrack = 4;

	FOnDataSourceDataUpdatedMulticastDelegate::FDelegate DataPoolChangedHandler;
};
