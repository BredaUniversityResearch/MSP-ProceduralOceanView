//#pragma once
//#include "EnvironmentSTateControllerComponent.h"
//
//#include "WaterBodyStateController.generated.h"
//
//class UEnvironmentStateSettings;
//class UGerstnerWaterWaveGeneratorSimple;
//UCLASS()
//class MSP_PROCEDURALGEN_API UWaterBodyStateController : public UActorComponent, public IEnvironmentStateControllerComponent
//{
//	GENERATED_BODY()
//public:
//	explicit UWaterBodyStateController(const FObjectInitializer&);
//
//	virtual void FindTargetComponents_Implementation() override;
//	virtual void UpdateTargetSettings_Implementation(float DeltaSeconds, const UEnvironmentStateSettings* Settings) override;
//
//private:
//	void InterpolateWaveGenerator(UGerstnerWaterWaveGeneratorSimple& Target, const UGerstnerWaterWaveGeneratorSimple& FromState, const UGerstnerWaterWaveGeneratorSimple& ToState, float a_InterpolateValue) const;
//
//	UPROPERTY(Transient)
//		class AWaterBody* WaterBody{ nullptr };
//	UPROPERTY(EditDefaultsOnly)
//		class UWaterBodyPresetCollection* WaterBodyPresets{ nullptr };
//	UPROPERTY(EditAnywhere)
//		float LerpTimeSeconds = 2.5f;
//	UPROPERTY(EditAnywhere)
//		float WindDirectionLerpFactor = 0.02f;
//
//	UPROPERTY(Transient, Instanced)
//		class UGerstnerWaterWaves* CurrentWaves = nullptr;
//	UPROPERTY(Transient, Instanced)
//		UGerstnerWaterWaveGeneratorSimple* CurrentWaveGenerator = nullptr;
//	UPROPERTY(VisibleAnywhere)
//		const class UWaterWavesAsset* CurrentWavesPreset = nullptr;
//	float CurrentWaterWavesPresetLerp = 0.0f;
//};