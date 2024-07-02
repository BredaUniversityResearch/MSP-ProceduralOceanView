#include "HeightFogStateController.h"

#include "EnvironmentStateSettings.h"
#include "MSPPawn.h"
#include "MSPPlayerCameraController.h"
#include "MSPPlayerController.h"
#include "MSPProcGenGameInstance.h"
#include "SkyCreatorActor.h"
#include "DataPool/DataPool.h"
#include "DataPool/DataPoolDataSourceNames.h"
#include "DataPool/DataPoolRaster.h"
#include "Kismet/GameplayStatics.h"
#include "MSP_ProceduralGen/MSP_ProceduralGen.h"
#include "MSPProcGenGameInstance.h"
#include "GenerationSettings.h"

namespace
{
	FSkyCreatorExponentialHeightFogSettings LerpFogSettings(const FSkyCreatorExponentialHeightFogSettings& From, const FSkyCreatorExponentialHeightFogSettings& To, float Alpha)
	{
		FSkyCreatorExponentialHeightFogSettings Result;

		Result.FogDensity = FMath::Lerp(From.FogDensity, To.FogDensity, Alpha);
		Result.FogHeightFalloff = FMath::Lerp(From.FogHeightFalloff, To.FogHeightFalloff, Alpha);
		Result.FogInscatteringColor = FMath::Lerp(From.FogInscatteringColor, To.FogInscatteringColor, Alpha);
		Result.FogStartDistance = FMath::Lerp(From.FogStartDistance, To.FogStartDistance, Alpha);

		Result.SecondFogDensity = FMath::Lerp(From.SecondFogDensity, To.SecondFogDensity, Alpha);
		Result.SecondFogHeightFalloff = FMath::Lerp(From.SecondFogHeightFalloff, To.SecondFogHeightFalloff, Alpha);

		Result.DirectionalInscatteringExponent = FMath::Lerp(From.DirectionalInscatteringExponent, To.DirectionalInscatteringExponent, Alpha);
		Result.DirectionalInscatteringColor = FMath::Lerp(From.DirectionalInscatteringColor, To.DirectionalInscatteringColor, Alpha);
		Result.DirectionalInscatteringStartDistance = FMath::Lerp(From.DirectionalInscatteringStartDistance, To.DirectionalInscatteringStartDistance, Alpha);

		Result.VolumetricFogScatteringDistribution = FMath::Lerp(From.VolumetricFogScatteringDistribution, To.VolumetricFogScatteringDistribution, Alpha);
		Result.VolumetricFogAlbedo = FMath::Lerp(From.VolumetricFogAlbedo, To.VolumetricFogAlbedo, Alpha);
		Result.VolumetricFogEmissive = FMath::Lerp(From.VolumetricFogEmissive, To.VolumetricFogEmissive, Alpha);
		Result.VolumetricFogExtinctionScale = FMath::Lerp(From.VolumetricFogExtinctionScale, To.VolumetricFogExtinctionScale, Alpha);

		return Result;
	}
}

const FName UHeightFogStateController::TurbidityMultiplierUIDataSourceName = FName("TurbidityMultiplierUI");

void UHeightFogStateController::FindTargetComponents_Implementation()
{
	if (TargetHeightFog == nullptr)
	{
		ASkyCreator* skyCreator = nullptr;
		TArray<AActor*> skyCreatorActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASkyCreator::StaticClass(), skyCreatorActors);
		if (skyCreatorActors.Num() == 1)
		{
			skyCreator = Cast<ASkyCreator>(skyCreatorActors[0]);
		}

		if (skyCreator != nullptr)
		{
			skyCreator->SetExponentialHeightFogSettings({});
			TargetHeightFog = skyCreator->GetComponentByClass<UExponentialHeightFogComponent>();
			if (TargetHeightFog != nullptr)
			{
				UE_LOG(LogMspProceduralGen, Warning, TEXT("Failed to find exponential height fog component in sky creator actor %s."), *skyCreator->GetName());
			}
		}
		else
		{
			if (!Cast<UMSPProcGenGameInstance>(GetWorld()->GetGameInstance())->GetGenerationSettings()->IsVR){
				UE_LOG(LogMspProceduralGen, Warning, TEXT("Failed to find a single instance of SkyCreator in the scene. Actually found %i instances."), skyCreatorActors.Num());
			}
		}
	}

}

void UHeightFogStateController::UpdateTargetSettings_Implementation(float DeltaSeconds, const UEnvironmentStateSettings* Settings)
{
	if (Settings == nullptr)
	{
		return;
	}

	if (TargetHeightFog == nullptr)
	{
		return;
	}

	bool cameraUnderWater = false; //Assume above water.
	FVector cameraLocation = FVector::Zero(); 
	AMSPPlayerController* controller = Settings->GetWorld()->GetFirstPlayerController<AMSPPlayerController>();
	if (const AMSPPlayerCameraManager* cameraManager = Cast<const AMSPPlayerCameraManager>(controller->PlayerCameraManager))
	{
		cameraLocation = cameraManager->GetCameraLocation();
		cameraUnderWater = cameraManager->IsCameraUnderWater();
	}

	if (cameraUnderWater == false)
	{
		if (LastVisibilityAmount != Settings->EnvironmentState.Visibility)
		{
			const FSkyCreatorExponentialHeightFogSettings targetFogSettings = LerpFogSettings(MinVisibilitySettings, MaxVisibilitySettings, Settings->EnvironmentState.Visibility);
			ApplyFogSettings(targetFogSettings);
			LastVisibilityAmount = Settings->EnvironmentState.Visibility;
		}
	}
	else
	{
		const UDataPool& dataPool = UMSPProcGenGameInstance::GetInstance()->GetDataPool();
		float turbidity = DefaultUnderwaterTurbidity;
		IDataPoolRasterReadOnly* turbidityRaster;
		if (dataPool.TryGetData(DataPoolDataSourceNames::TURBIDITY, turbidityRaster))
		{
			turbidity = static_cast<float>(turbidityRaster->GetMappedValueAtWorldPosition(FVector2D(cameraLocation.X, cameraLocation.Y)));
		}

		float turbidityMultiplier = 1.0f;
		dataPool.TryGetData(TurbidityMultiplierUIDataSourceName, turbidityMultiplier);

		ApplyFogSettings(LerpFogSettings(UnderwaterMinTurbidityFogSettings, UnderwaterMaxTurbidityFogSettings, turbidity * turbidityMultiplier));
	}
}

void UHeightFogStateController::ApplyFogSettings(const FSkyCreatorExponentialHeightFogSettings& TargetFogSettings) const
{
	TargetHeightFog->SetFogDensity(TargetFogSettings.FogDensity);
	TargetHeightFog->SetFogHeightFalloff(TargetFogSettings.FogHeightFalloff);
	TargetHeightFog->SetFogInscatteringColor(TargetFogSettings.FogInscatteringColor);
	TargetHeightFog->SetStartDistance(TargetFogSettings.FogStartDistance);

	TargetHeightFog->SecondFogData.FogDensity = TargetFogSettings.SecondFogDensity;
	TargetHeightFog->SecondFogData.FogHeightFalloff = TargetFogSettings.SecondFogHeightFalloff;

	TargetHeightFog->SetDirectionalInscatteringExponent(TargetFogSettings.DirectionalInscatteringExponent);
	TargetHeightFog->SetDirectionalInscatteringColor(TargetFogSettings.DirectionalInscatteringColor);
	TargetHeightFog->SetDirectionalInscatteringStartDistance(TargetFogSettings.DirectionalInscatteringStartDistance);

	TargetHeightFog->SetVolumetricFogScatteringDistribution(TargetFogSettings.VolumetricFogScatteringDistribution);
	TargetHeightFog->SetVolumetricFogAlbedo(TargetFogSettings.VolumetricFogAlbedo.ToFColor(true));
	TargetHeightFog->SetVolumetricFogEmissive(TargetFogSettings.VolumetricFogEmissive);
	TargetHeightFog->SetVolumetricFogExtinctionScale(TargetFogSettings.VolumetricFogExtinctionScale);
	TargetHeightFog->MarkRenderStateDirty();
}