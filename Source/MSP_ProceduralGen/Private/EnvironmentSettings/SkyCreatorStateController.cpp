#include "SkyCreatorStateController.h"

#include "EnvironmentStateSettings.h"
#include "MSPCameraWetnessController.h"
#include "MSPPlayerCameraController.h"
#include "SkyCreatorActor.h"
#include "SkyCreatorPresetCollection.h"
#include "Kismet/GameplayStatics.h"
#include "MSP_ProceduralGen/MSP_ProceduralGen.h"
#include "MSPProcGenGameInstance.h"
#include "GenerationSettings.h"

void USkyCreatorStateController::FindTargetComponents_Implementation()
{
	//if (SkyCreator == nullptr)
	//{
	//	TArray<AActor*> skyCreatorActors;
	//	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASkyCreator::StaticClass(), skyCreatorActors);
	//	if (skyCreatorActors.Num() == 1)
	//	{
	//		SkyCreator = Cast<ASkyCreator>(skyCreatorActors[0]);
	//	}
	//
	//	if (SkyCreator != nullptr)
	//	{
	//		SkyCreator->SetExponentialHeightFogSettings({});
	//		TargetHeightFog = SkyCreator->GetComponentByClass<UExponentialHeightFogComponent>();
	//		if (TargetHeightFog != nullptr)
	//		{
	//			UE_LOG(LogMspProceduralGen, Warning, TEXT("Failed to find exponential height fog component in sky creator actor %s."), *SkyCreator->GetName());
	//		}
	//	}
	//	else
	//	{
	//		if (!Cast<UMSPProcGenGameInstance>(GetWorld()->GetGameInstance())->GetGenerationSettings()->IsVR){
	//			UE_LOG(LogMspProceduralGen, Warning, TEXT("Failed to find a single instance of SkyCreator in the scene. Actually found %i instances."), skyCreatorActors.Num());
	//		}
	//	}
	//}
	//
	//if (SkyCreatorPresets == nullptr)
	//{
	//	UE_LOG(LogMspProceduralGen, Warning, TEXT("No SkyCreator preset collection was applied to the environment state controller"));
	//}
	//
	//if (TargetCameraManager == nullptr)
	//{
	//	APlayerController* playerController = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), 0);
	//	if (playerController != nullptr)
	//	{
	//		TargetCameraManager = Cast<AMSPPlayerCameraManager>(playerController->PlayerCameraManager);
	//		if (TargetCameraManager == nullptr)
	//		{
	//			UE_LOG(LogMspProceduralGen, Warning, TEXT("No MSPPlayerCameraManager found."));
	//		}
	//	}
	//}
}

void USkyCreatorStateController::UpdateTargetSettings_Implementation(float DeltaSeconds, const UEnvironmentStateSettings* Settings)
{
	//if (Settings == nullptr)
	//{
	//	return;
	//}
	//
	//UpdateHeightFogSettings(DeltaSeconds, Settings);
	//
	//if (SkyCreatorPresets == nullptr || SkyCreator == nullptr)
	//{
	//	return;
	//}
	//
	//SkyCreator->SetTime(Settings->EnvironmentState.TimeOfDay * 24.0f);
	//
	//const USkyCreatorWeatherPreset* targetPreset = SkyCreatorPresets->FindBestMatchPresetForConditions(Settings->EnvironmentState);
	//if (targetPreset != nullptr && CurrentSkyCreatorPreset != targetPreset)
	//{
	//	CurrentSkyCreatorPreset = targetPreset;
	//	CurrentSkyCreatorPresetLerp = 0.0f;
	//	LerpStartSettings = SkyCreator->GetWeatherSettings();
	//}
	//
	//if (CurrentSkyCreatorPresetLerp >= 1.0f)
	//{
	//	return;
	//}
	//
	//CurrentSkyCreatorPresetLerp = FMath::Min(CurrentSkyCreatorPresetLerp + (DeltaSeconds / LerpTimeSeconds), 1.0f);
	//
	//FSkyCreatorWeatherSettings targetWeatherSettings = targetPreset->GetWeatherPresetSettings();
	//
	//SkyCreator->LerpSkyAtmosphereSettings(LerpStartSettings.SkyAtmosphereSettings, targetWeatherSettings.SkyAtmosphereSettings, CurrentSkyCreatorPresetLerp);
	//SkyCreator->LerpVolumetricCloudSettings(LerpStartSettings.VolumetricCloudSettings, targetWeatherSettings.VolumetricCloudSettings, CurrentSkyCreatorPresetLerp);
	//SkyCreator->LerpBackgroundCloudSettings(LerpStartSettings.BackgroundCloudSettings, targetWeatherSettings.BackgroundCloudSettings, CurrentSkyCreatorPresetLerp);
	//SkyCreator->LerpSkyLightSettings(LerpStartSettings.SkyLightSettings, targetWeatherSettings.SkyLightSettings, CurrentSkyCreatorPresetLerp);
	//SkyCreator->LerpSunLightSettings(LerpStartSettings.SunLightSettings, targetWeatherSettings.SunLightSettings, CurrentSkyCreatorPresetLerp);
	//SkyCreator->LerpMoonLightSettings(LerpStartSettings.MoonLightSettings, targetWeatherSettings.MoonLightSettings, CurrentSkyCreatorPresetLerp);
	////SkyCreator->LerpExponentialHeightFogSettings(LerpStartSettings.ExponentialHeightFogSettings, targetWeatherSettings.ExponentialHeightFogSettings, CurrentSkyCreatorPresetLerp);
	//SkyCreator->LerpStarMapSettings(LerpStartSettings.StarMapSettings, targetWeatherSettings.StarMapSettings, CurrentSkyCreatorPresetLerp);
	//SkyCreator->LerpWeatherFXSettings(LerpStartSettings.WeatherFXSettings, targetWeatherSettings.WeatherFXSettings, CurrentSkyCreatorPresetLerp);
	//SkyCreator->LerpWeatherMaterialFXSettings(LerpStartSettings.WeatherMaterialFXSettings, targetWeatherSettings.WeatherMaterialFXSettings, CurrentSkyCreatorPresetLerp);
	//SkyCreator->LerpWindSettings(LerpStartSettings.WindSettings, targetWeatherSettings.WindSettings, CurrentSkyCreatorPresetLerp);
	//SkyCreator->LerpPostProcessSettings(LerpStartSettings.PostProcessSettings, targetWeatherSettings.PostProcessSettings, CurrentSkyCreatorPresetLerp);
	//
	//if (TargetCameraManager != nullptr && TargetCameraManager->CameraWetnessController != nullptr)
	//{
	//	const FSkyCreatorWeatherFXSettings weatherSettings = SkyCreator->GetWeatherFXSettings();
	//	TargetCameraManager->CameraWetnessController->RequestWetness(128454534u, weatherSettings.RainAmount);
	//}
}

void USkyCreatorStateController::UpdateHeightFogSettings(float DeltaSeconds, const UEnvironmentStateSettings* Settings) const
{
	//if (TargetHeightFog == nullptr)
	//{
	//	return;
	//}
	//
	//TargetHeightFog->SetFogDensity(FMath::Lerp(FogMinDensity, FogMaxDensity, (1.0f - Settings->EnvironmentState.Visibility)));
}
