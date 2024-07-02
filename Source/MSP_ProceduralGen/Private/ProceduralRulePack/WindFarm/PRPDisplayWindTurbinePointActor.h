// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralRulePack/GenericDisplayModes/PRPDisplayModePointActor.h"
#include "PRPDisplayWindTurbinePointActor.generated.h"

/**
 * 
 */
UCLASS()
class UPRPDisplayWindTurbinePointActor : public UPRPDisplayModePointActor
{
	GENERATED_BODY()
	
public:
	inline static const FName WIND_SPEED_TO_KWH_FACTOR= "wind_speed_to_kwh_factor";
	inline static const FName WIND_SPEED_TO_BLADE_ROTATION_FACTOR = "wind_speed_to_blade_rotation_factor";
	inline static const FName MIN_OPERATIONAL_WIND_SPEED = "min_operational_wind_speed";
	inline static const FName MAX_OPERATIONAL_WIND_SPEED = "max_operational_wind_speed";
	inline static const FName WIND_TURBINE_DESCRIPTION =   "wind_turbine_description";

	// Constructor
	UPRPDisplayWindTurbinePointActor();

	virtual void EnableDisplayMode_Implementation(UDataPool* a_dataPool) override;
	virtual void DisableDisplayMode_Implementation(UDataPool* a_dataPool) override;

private:
	
	UPROPERTY(EditDefaultsOnly, Category = "Turbine Specs")
		float WindSpeedToKWHFactor = 124.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Turbine Specs")
		float WindToBladeRotationSpeedFactor = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Turbine Specs")
		float MinOperationalWindSpeedKnots = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Turbine Specs")
		float MaxOperationalWindSpeedKnots = 48.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Turbine Specs", meta = (MultiLine = "true"))
		FText WindTurbineDescription = FText::GetEmpty();

};
