#pragma once
#include "EnvironmentStateControllerComponent.h"
#include "SkyCreatorWeatherSettings.h"
#include "HeightFogStateController.generated.h"

UCLASS(BlueprintType)
class MSP_PROCEDURALGEN_API UHeightFogStateController : public UActorComponent, public IEnvironmentStateControllerComponent 
{
	GENERATED_BODY()

	static constexpr float SeaLevelHeight = 0.0f;
	static const FName TurbidityMultiplierUIDataSourceName;

public:
	virtual void FindTargetComponents_Implementation() override;
	virtual void UpdateTargetSettings_Implementation(float DeltaSeconds, const UEnvironmentStateSettings* Settings) override;

private:
	void ApplyFogSettings(const FSkyCreatorExponentialHeightFogSettings& TargetFogSettings) const;

	UPROPERTY(Transient)
		UExponentialHeightFogComponent* TargetHeightFog {nullptr};

		float LastVisibilityAmount = {-1.0f};
	UPROPERTY(EditAnywhere)
		FSkyCreatorExponentialHeightFogSettings MinVisibilitySettings;
	UPROPERTY(EditAnywhere)
		FSkyCreatorExponentialHeightFogSettings MaxVisibilitySettings;
	UPROPERTY(EditAnywhere)
		float DefaultUnderwaterTurbidity = 1.0f;
	UPROPERTY(EditAnywhere)
		FSkyCreatorExponentialHeightFogSettings UnderwaterMinTurbidityFogSettings;
	UPROPERTY(EditAnywhere)
		FSkyCreatorExponentialHeightFogSettings UnderwaterMaxTurbidityFogSettings;
};