#pragma once

#include "MSPCameraWetnessController.generated.h"

UCLASS(BlueprintType)
class UMSPCameraWetnessController: public UActorComponent
{
	GENERATED_BODY()
	struct FWetnessEntry
	{
		uint32 Identifier;
		float Value;
	};

public:
	UMSPCameraWetnessController(const FObjectInitializer& ObjectInitializer);

	void BeginPlay();
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void RequestWetness(uint32 Identifier, float RequestedWetness);

private:
	UPROPERTY(Transient)
	class APostProcessVolume* TargetPostProcessVolume{ nullptr };
	UPROPERTY(EditAnywhere)
	UMaterialInterface* CameraWetnessMaterialAsset{ nullptr };
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* CameraWetnessMaterialInstance {nullptr};

	UPROPERTY(Transient,VisibleAnywhere)
	bool CameraWetnessAllowed {true};

	TArray<FWetnessEntry> ActiveWetnessEntries;
};