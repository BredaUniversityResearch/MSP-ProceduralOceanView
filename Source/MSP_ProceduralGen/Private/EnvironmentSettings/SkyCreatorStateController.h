#pragma once
#include "SkyCreatorWeatherSettings.h"
#include "EnvironmentSTateControllerComponent.h"
#include "SkyCreatorStateController.generated.h"

UCLASS(BlueprintType)
class MSP_PROCEDURALGEN_API USkyCreatorStateController : public UActorComponent, public IEnvironmentStateControllerComponent 
{
	GENERATED_BODY()
public:
	virtual void FindTargetComponents_Implementation() override;
	virtual void UpdateTargetSettings_Implementation(float DeltaSeconds, const UEnvironmentStateSettings* Settings) override;

private:
	void UpdateHeightFogSettings(float DeltaSeconds, const UEnvironmentStateSettings* Settings) const;

	//UPROPERTY(Transient)
	//class ASkyCreator* SkyCreator{ nullptr };
	//UPROPERTY(Transient)
	//UExponentialHeightFogComponent* TargetHeightFog {nullptr};
	//UPROPERTY(Transient)
	//class AMSPPlayerCameraManager* TargetCameraManager {nullptr};
	//UPROPERTY(EditAnywhere)
	//float LerpTimeSeconds = 10.0f;
	//
	//UPROPERTY(EditAnywhere)
	//float FogMinDensity = 0.002f;
	//UPROPERTY(EditAnywhere)
	//float FogMaxDensity = 0.06f;
	//
	//UPROPERTY(EditDefaultsOnly)
	//class USkyCreatorPresetCollection* SkyCreatorPresets{ nullptr };
	//UPROPERTY(VisibleAnywhere)
	//const class USkyCreatorWeatherPreset* CurrentSkyCreatorPreset = nullptr;

	//float CurrentSkyCreatorPresetLerp = 0.0f;
	//FSkyCreatorWeatherSettings LerpStartSettings{};
};