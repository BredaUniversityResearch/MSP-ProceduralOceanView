// Fill out your copyright notice in the Description page of Project Settings.
#include "ProceduralRulePack/Fish/PRPDataSourceProcessorFish.h"

#include "DataPool/DataPool.h"
#include "DataPool/DataPoolRaster.h"

#include "DataPool/DataPoolDataSourceNames.h"

void UPRPDataSourceProcessorFish::RegisterWithDataPool_Implementation(UDataPool* DataPool)
{
    Super::RegisterWithDataPool_Implementation(DataPool);

    m_dataPoolChangedDelegate.BindDynamic(this, &UPRPDataSourceProcessorFish::OnDataSourceChanged);
    const bool regSuccessNoise = DataPool->TryRegisterDataUpdatedListener(DataPoolDataSourceNames::NOISE, m_dataPoolChangedDelegate);
    check(regSuccessNoise);

    const bool regSuccessFish = DataPool->TryRegisterDataUpdatedListener(DataPoolDataSourceNames::FISH_DENSITY_COD, m_dataPoolChangedDelegate);
    check(regSuccessFish);

    const bool createdDataLayer = DataPool->CreateDataSource(FishPlacementDataSourceName, EDataPoolDataType::Points, EDataPoolDataUnitType::Other);
    check(createdDataLayer)

    IDataPoolRasterReadOnly* noiseData = nullptr;
    IDataPoolRasterReadOnly* fishDensityData = nullptr;
    if (DataPool->TryGetData(DataPoolDataSourceNames::NOISE, noiseData) && DataPool->TryGetData(DataPoolDataSourceNames::FISH_DENSITY_COD, fishDensityData))
    {
        RecalculatePlacementData(noiseData, fishDensityData);
    }
}

void UPRPDataSourceProcessorFish::OnDataSourceChanged(const FName& dataSourceName)
{
    if (dataSourceName != DataPoolDataSourceNames::NOISE && dataSourceName != DataPoolDataSourceNames::FISH_DENSITY_COD)
    {
        return;
    }

    IDataPoolRasterReadOnly* noiseData = nullptr;
    IDataPoolRasterReadOnly* fishDensityData = nullptr;

    const bool resultNoise = TargetDataPool->TryGetData(DataPoolDataSourceNames::NOISE, noiseData);
    const bool resultFish = TargetDataPool->TryGetData(DataPoolDataSourceNames::FISH_DENSITY_COD, fishDensityData);
    ensure(resultNoise && resultFish);

    RecalculatePlacementData( noiseData, fishDensityData);
}

void UPRPDataSourceProcessorFish::RecalculatePlacementData( const IDataPoolRasterReadOnly* noiseData, const IDataPoolRasterReadOnly* fishDensityData)
{

    int32 sizeX = -1;
    int32 sizeY = -1;

    int32 fishMinValue = 0;
    int32 fishMaxValue = 0;

    //FIntPoint rasterSize = fishDensityData->GetRasterSize();
    //ensure(sizeX > 0 && sizeY > 0);

    //fishDensityData->GetRawRangeValues(fishMinValue, fishMaxValue);
    //ensure(fishMinValue >= 0.0f && fishMaxValue >= 0.0f);

    //FDataPoolPoints fishPlacementData = {};

    ////find the peak coords in fish density data
    //auto peakCoords = FInt32Vector2(-1,-1);

    //for (int32 x = 0; x < sizeX; ++x)
    //{
    //    for (int32 y = 0; y < sizeY; ++y)
    //    {
    //        uint8 value = fishDensityData->GetRawValue(x, y);
    //        if (value == fishMaxValue)
    //        {
    //            peakCoords = FInt32Vector2(x, y);
    //            break;
    //        }
    //    }
    //    if (peakCoords.X != -1 && peakCoords.Y != -1)
    //    {
    //        break;
    //    }
    //}
}


