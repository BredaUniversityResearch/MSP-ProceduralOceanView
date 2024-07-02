#include "ProceduralRulePackActor.h"

#include "MSPObjectFadeControllerComponent.h"

#include "Components/MeshComponent.h"


AProceduralRulePackActor::AProceduralRulePackActor()
{
    PrimaryActorTick.bCanEverTick = true;

    FadeController = CreateDefaultSubobject<UMSPObjectFadeControllerComponent>(TEXT("FadeController"));

}

void AProceduralRulePackActor::PRPActorSetupComplete_Implementation()
{
}

void AProceduralRulePackActor::OnLayerPropertiesChanged()
{
}

void AProceduralRulePackActor::OnStartDespawn_Implementation()
{
	TArray<UMaterialInstanceDynamic*> AllMaterials;

    TArray<UMeshComponent*> MeshComponents;
	GetComponents<UMeshComponent>( MeshComponents );

	for (auto MeshComponent : MeshComponents)
	{
		auto NumMaterials = MeshComponent->GetNumMaterials();
		
		for (int i = 0; i < NumMaterials; i++)
		{
			auto Material = MeshComponent->GetMaterial(i);
			auto MaterialDynamic = Cast<UMaterialInstanceDynamic>(Material);
			if (MaterialDynamic != nullptr)
			{
				AllMaterials.Add(MaterialDynamic);
			}else
			{
				auto matInst = UMaterialInstanceDynamic::Create(Material, this);
				MeshComponent->SetMaterial(i, matInst);
				AllMaterials.Add(matInst);
			}
		}
	}

    FadeController->StartFadeOut(AllMaterials);

}

FString AProceduralRulePackActor::GetPOVInfo_Implementation()
{
    return FString();
}
