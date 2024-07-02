#include "MSPPlayerCameraController.h"

#include "MSPCameraWetnessController.h"

AMSPPlayerCameraManager::AMSPPlayerCameraManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	CameraWetnessController = CreateDefaultSubobject<UMSPCameraWetnessController>(FName("CameraWetnessController"));
}

void AMSPPlayerCameraManager::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const FMinimalViewInfo& view = GetCameraCacheView();
	bool isUnderWater = view.Location.Z < 0.0f;

	if (!isUnderWater && CameraWasUnderWaterLastFrame)
	{
		CameraCurrentWetness = 1.0f;
	}
	CameraWasUnderWaterLastFrame = isUnderWater;

	CameraCurrentWetness = FMath::Max(0.0, CameraCurrentWetness - ((1.0f / CameraWetnessExitWaterDuration) * DeltaSeconds));
	CameraWetnessController->RequestWetness(765745635ul, CameraCurrentWetness);
}

bool AMSPPlayerCameraManager::IsCameraUnderWater() const
{
	return CameraWasUnderWaterLastFrame;
}

float AMSPPlayerCameraManager::GetCameraHeight() const
{
    return GetCameraCacheView().Location.Z;
}
