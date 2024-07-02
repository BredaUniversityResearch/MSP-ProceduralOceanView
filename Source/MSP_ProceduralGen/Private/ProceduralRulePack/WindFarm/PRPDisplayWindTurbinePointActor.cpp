// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralRulePack/WindFarm/PRPDisplayWindTurbinePointActor.h"
#include "DataPool/DataPool.h"
#include <MSPProcGenGameInstance.h>

#define internName

void UPRPDisplayWindTurbinePointActor::EnableDisplayMode_Implementation(UDataPool* a_dataPool)
{
	// Create the Custom Data source if they don't exit
	UMSPProcGenGameInstance::GetInstance()->GetDataPool().CreateDataSource(WIND_SPEED_TO_KWH_FACTOR, EDataPoolDataType::Float, EDataPoolDataUnitType::Other);
	UMSPProcGenGameInstance::GetInstance()->GetDataPool().CreateDataSource(WIND_SPEED_TO_BLADE_ROTATION_FACTOR, EDataPoolDataType::Float, EDataPoolDataUnitType::Other);
	UMSPProcGenGameInstance::GetInstance()->GetDataPool().CreateDataSource(MIN_OPERATIONAL_WIND_SPEED, EDataPoolDataType::Float, EDataPoolDataUnitType::Knots);
	UMSPProcGenGameInstance::GetInstance()->GetDataPool().CreateDataSource(MAX_OPERATIONAL_WIND_SPEED, EDataPoolDataType::Float, EDataPoolDataUnitType::Knots);
	UMSPProcGenGameInstance::GetInstance()->GetDataPool().CreateDataSource(WIND_TURBINE_DESCRIPTION, EDataPoolDataType::String, EDataPoolDataUnitType::Other);

	// Update the data sources related to this Display mode
	UMSPProcGenGameInstance::GetInstance()->GetDataPool().TrySetData(WIND_SPEED_TO_KWH_FACTOR, WindSpeedToKWHFactor);
	UMSPProcGenGameInstance::GetInstance()->GetDataPool().TrySetData(WIND_SPEED_TO_BLADE_ROTATION_FACTOR, WindToBladeRotationSpeedFactor);
	UMSPProcGenGameInstance::GetInstance()->GetDataPool().TrySetData(MIN_OPERATIONAL_WIND_SPEED, MinOperationalWindSpeedKnots);
	UMSPProcGenGameInstance::GetInstance()->GetDataPool().TrySetData(MAX_OPERATIONAL_WIND_SPEED, MaxOperationalWindSpeedKnots);
	UMSPProcGenGameInstance::GetInstance()->GetDataPool().TrySetData(WIND_TURBINE_DESCRIPTION, WindTurbineDescription.ToString());

	// Create asset instances
	UPRPDisplayModePointActor::EnableDisplayMode_Implementation(a_dataPool);
}

void UPRPDisplayWindTurbinePointActor::DisableDisplayMode_Implementation(UDataPool* a_dataPool)
{
	UPRPDisplayModePointActor::DisableDisplayMode_Implementation(a_dataPool);
}

UPRPDisplayWindTurbinePointActor::UPRPDisplayWindTurbinePointActor()
{
}
