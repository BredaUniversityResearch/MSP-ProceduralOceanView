// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DataPool/DataPool.h"
#include "ProceduralRulePack/ProceduralRulePackDataSourceProcessor.h"
#include "PRPDataSourceProcessorFish.generated.h"

class UDataPoolRaster;

/**
 * 
 */
UCLASS()
class UPRPDataSourceProcessorFish : public UProceduralRulePackDataSourceProcessor
{
	GENERATED_BODY()

public: 
	void RegisterWithDataPool_Implementation(UDataPool* DataPool) override;

	UFUNCTION()
	void OnDataSourceChanged(const FName& DataSourceName);

private:
	void RecalculatePlacementData(const IDataPoolRasterReadOnly* noiseData, const IDataPoolRasterReadOnly* fishDensityData);

	FName FishPlacementDataSourceName = "INVALLID";

	FOnDataSourceDataUpdatedMulticastDelegate::FDelegate m_dataPoolChangedDelegate;

};
