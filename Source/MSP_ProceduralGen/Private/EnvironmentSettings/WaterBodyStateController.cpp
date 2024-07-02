//#include "WaterBodyStateController.h"
//
//#include "EnvironmentStateSettings.h"
//#include "GerstnerWaterWaves.h"
//#include "WaterBodyActor.h"
//#include "WaterBodyPresetCollection.h"
//#include "Kismet/GameplayStatics.h"
//#include "MSP_ProceduralGen/MSP_ProceduralGen.h"
//
//
//UWaterBodyStateController::UWaterBodyStateController(const FObjectInitializer&)
//{
//	CurrentWaves = CreateDefaultSubobject<UGerstnerWaterWaves>("CurrentWaves", true);
//	CurrentWaves->GerstnerWaveGenerator = CreateDefaultSubobject<UGerstnerWaterWaveGeneratorSimple>("CurrentWaves_WaveGenerator", true);
//	CurrentWaveGenerator = CreateDefaultSubobject<UGerstnerWaterWaveGeneratorSimple>("CurrentWaveGenerator", true);
//}
//
//void UWaterBodyStateController::FindTargetComponents_Implementation()
//{
//	if (WaterBody == nullptr)
//	{
//		TArray<AActor*> waterBodyActors;
//		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaterBody::StaticClass(), waterBodyActors);
//		if (waterBodyActors.Num() == 1)
//		{
//			WaterBody = Cast<AWaterBody>(waterBodyActors[0]);
//			WaterBody->SetWaterWaves(CurrentWaves);
//		}
//
//		if (WaterBody == nullptr)
//		{
//			UE_LOG(LogMspProceduralGen, Warning, TEXT("Failed to find a single instance of AWaterBody in the scene. Actually found %i instances."), waterBodyActors.Num());
//		}
//	}
//
//	if (WaterBodyPresets == nullptr)
//	{
//		UE_LOG(LogMspProceduralGen, Warning, TEXT("No WaterBody preset collection was applied to the environment state controller"));
//	}
//}
//
//void UWaterBodyStateController::UpdateTargetSettings_Implementation(float DeltaSeconds, const UEnvironmentStateSettings* Settings)
//{
//	if (Settings == nullptr)
//	{
//		return;
//	}
//
//	if (WaterBodyPresets == nullptr || WaterBody == nullptr)
//	{
//		return;
//	}
//
//	UGerstnerWaterWaveGeneratorSimple* waveGenerator = Cast<UGerstnerWaterWaveGeneratorSimple>(CurrentWaves->GerstnerWaveGenerator);
//	const UWaterWavesAsset* targetPreset = WaterBodyPresets->FindBestMatchPresetForConditions(Settings->EnvironmentState);
//	if (targetPreset != nullptr && CurrentWavesPreset != targetPreset)
//	{
//		if (targetPreset == nullptr)
//		{
//			//First setup, copy settings to interpolation settings and actual settings.
//			const UGerstnerWaterWaves* targetWaves = Cast<UGerstnerWaterWaves>(targetPreset->GetWaterWaves());
//			ensureMsgf(targetWaves != nullptr, TEXT("Target preset asset has unknown waves type"));
//			const UGerstnerWaterWaveGeneratorSimple* presetWaveGenerator = Cast<UGerstnerWaterWaveGeneratorSimple>(targetWaves->GerstnerWaveGenerator);
//			ensureMsgf(presetWaveGenerator != nullptr, TEXT("Target wave generator has unknown type"));
//
//			InterpolateWaveGenerator(*CurrentWaveGenerator, *waveGenerator, *presetWaveGenerator, 1.0f);
//			InterpolateWaveGenerator(*waveGenerator, *CurrentWaveGenerator, *CurrentWaveGenerator, 1.0f);
//			CurrentWaterWavesPresetLerp = 1.0f;
//		}
//		else
//		{
//			InterpolateWaveGenerator(*CurrentWaveGenerator, *waveGenerator, *waveGenerator, 0.0f);
//			CurrentWaterWavesPresetLerp = 0.0f;
//		}
//		CurrentWavesPreset = targetPreset;
//		CurrentWaterWavesPresetLerp = 0.0f;
//	}
//
//	bool needsWaveRecompute = false;
//	if (CurrentWaterWavesPresetLerp <= 1.0f)
//	{
//		CurrentWaterWavesPresetLerp = FMath::Min(CurrentWaterWavesPresetLerp + (DeltaSeconds / LerpTimeSeconds), 1.0f);
//
//		const UGerstnerWaterWaves* targetWaves = Cast<UGerstnerWaterWaves>(CurrentWavesPreset->GetWaterWaves());
//		ensureMsgf(targetWaves != nullptr, TEXT("Target preset asset has unknown waves type"));
//		const UGerstnerWaterWaveGeneratorSimple* presetWaveGenerator = Cast<UGerstnerWaterWaveGeneratorSimple>(targetWaves->GerstnerWaveGenerator);
//		ensureMsgf(presetWaveGenerator != nullptr, TEXT("Target wave generator has unknown type"));
//		InterpolateWaveGenerator(*waveGenerator, *CurrentWaveGenerator, *presetWaveGenerator, CurrentWaterWavesPresetLerp);
//		needsWaveRecompute = true;
//	}
//
//	float newWindDirection = FMath::Lerp(waveGenerator->WindAngleDeg, Settings->EnvironmentState.WindDirection, WindDirectionLerpFactor);
//	if (!FMath::IsNearlyEqual(newWindDirection, waveGenerator->WindAngleDeg))
//	{
//		waveGenerator->WindAngleDeg = newWindDirection;
//		needsWaveRecompute = true;
//	}
//
//	if (needsWaveRecompute)
//	{
//		CurrentWaves->RecomputeWaves(true);
//	}
//}
//
//void UWaterBodyStateController::InterpolateWaveGenerator(UGerstnerWaterWaveGeneratorSimple& Target, const UGerstnerWaterWaveGeneratorSimple& FromState, const UGerstnerWaterWaveGeneratorSimple& ToState, float a_InterpolateValue) const
//{
//	Target.NumWaves = FMath::Lerp(FromState.NumWaves, ToState.NumWaves, a_InterpolateValue);
//	//Target.Seed = ToState.Seed;
//	Target.Randomness = FMath::Lerp(FromState.Randomness, ToState.Randomness, a_InterpolateValue);
//
//	Target.MinWavelength = FMath::Lerp(FromState.MinWavelength, ToState.MinWavelength, a_InterpolateValue);
//	Target.MaxWavelength = FMath::Lerp(FromState.MaxWavelength, ToState.MaxWavelength, a_InterpolateValue);
//	Target.WavelengthFalloff = FMath::Lerp(FromState.WavelengthFalloff, ToState.WavelengthFalloff, a_InterpolateValue);
//
//	Target.MinAmplitude = FMath::Lerp(FromState.MinAmplitude, ToState.MinAmplitude, a_InterpolateValue);
//	Target.MaxAmplitude = FMath::Lerp(FromState.MaxAmplitude, ToState.MaxAmplitude, a_InterpolateValue);
//	Target.AmplitudeFalloff = FMath::Lerp(FromState.AmplitudeFalloff, ToState.AmplitudeFalloff, a_InterpolateValue);
//
//	Target.WindAngleDeg = FMath::Lerp(FromState.WindAngleDeg, ToState.WindAngleDeg, a_InterpolateValue);
//
//	Target.DirectionAngularSpreadDeg = FMath::Lerp(FromState.DirectionAngularSpreadDeg, ToState.DirectionAngularSpreadDeg, a_InterpolateValue);
//
//	Target.SmallWaveSteepness = FMath::Lerp(FromState.SmallWaveSteepness, ToState.SmallWaveSteepness, a_InterpolateValue);
//	Target.LargeWaveSteepness = FMath::Lerp(FromState.LargeWaveSteepness, ToState.LargeWaveSteepness, a_InterpolateValue);
//	Target.SteepnessFalloff = FMath::Lerp(FromState.SteepnessFalloff, ToState.SteepnessFalloff, a_InterpolateValue);
//}