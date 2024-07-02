#include "MSPCameraWetnessController.h"

#include "GenerationSettings.h"
#include "MSPProcGenGameInstance.h"
#include "Engine/PostProcessVolume.h"
#include "Kismet/GameplayStatics.h"
#include "MSP_ProceduralGen/MSP_ProceduralGen.h"

UMSPCameraWetnessController::UMSPCameraWetnessController(const FObjectInitializer& ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMSPCameraWetnessController::BeginPlay()
{
	Super::BeginPlay();

	CameraWetnessAllowed = !UMSPProcGenGameInstance::GetInstance()->GetGenerationSettings()->IsVR;

	TArray<AActor*> postProcessVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APostProcessVolume::StaticClass(), postProcessVolumes);
	if (postProcessVolumes.Num() == 1)
	{
		TargetPostProcessVolume = Cast<APostProcessVolume>(postProcessVolumes[0]);
	}

	if (TargetPostProcessVolume == nullptr)
	{
		UE_LOG(LogMspProceduralGen, Warning, TEXT("Failed to find a single instance of APostProcessVolume in the scene. Actually found %i instances."), postProcessVolumes.Num());
	}
	else
	{
		TArray<FWeightedBlendable>& blendables = TargetPostProcessVolume->Settings.WeightedBlendables.Array;
		for(FWeightedBlendable& blendable : blendables)
		{
			UMaterialInterface* material = Cast<UMaterialInterface>(blendable.Object);
			if (material != nullptr && material->GetMaterial() == CameraWetnessMaterialAsset->GetMaterial())
			{
				CameraWetnessMaterialInstance = UMaterialInstanceDynamic::Create(material, TargetPostProcessVolume);
				blendable.Object = CameraWetnessMaterialInstance;
			}
		}
	}
}

void UMSPCameraWetnessController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CameraWetnessMaterialInstance != nullptr)
	{
		float maxTargetWetness = 0.0f;
		if (CameraWetnessAllowed)
		{
			for (const FWetnessEntry& entry : ActiveWetnessEntries)
			{
				maxTargetWetness = FMath::Max(maxTargetWetness, entry.Value);
			}
		}

		CameraWetnessMaterialInstance->SetScalarParameterValue("WetnessCamera", maxTargetWetness);
	}
}

void UMSPCameraWetnessController::RequestWetness(uint32 Identifier, float RequestedWetness)
{
	int index = ActiveWetnessEntries.IndexOfByPredicate([Identifier](const FWetnessEntry& Entry) { return Entry.Identifier == Identifier; });
	if (index != INDEX_NONE)
	{
		if (RequestedWetness > 0)
		{
			ActiveWetnessEntries[index].Value = RequestedWetness;
		}
		else
		{
			ActiveWetnessEntries.RemoveAtSwap(index);
		}
	}
	else if (RequestedWetness > 0)
	{
		FWetnessEntry entry = {
			.Identifier = Identifier,
			.Value = RequestedWetness,
		};
		ActiveWetnessEntries.Emplace(entry);
	}
}
