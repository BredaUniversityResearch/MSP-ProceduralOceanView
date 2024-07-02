#pragma once

#include "MSPPlayerCameraController.generated.h"

class UMSPCameraWetnessController;
class UNiagaraSelectionViaRenderBackBufferComponent;

UCLASS()
class AMSPPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
public:
	AMSPPlayerCameraManager(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsCameraUnderWater() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetCameraHeight() const;

	UPROPERTY(EditAnywhere)
	float CameraWetnessExitWaterDuration {5.0f};
	float CameraCurrentWetness{0.0f};
	bool CameraWasUnderWaterLastFrame{false};



	UPROPERTY(EditAnywhere)
	UMSPCameraWetnessController* CameraWetnessController{nullptr};
};