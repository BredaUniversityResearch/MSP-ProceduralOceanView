
#include "MSPObjectFadeControllerComponent.h"

#include "Components/MeshComponent.h"

UMSPObjectFadeControllerComponent::UMSPObjectFadeControllerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;   
}

void UMSPObjectFadeControllerComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!EnableFadeController)
    {
        return;
    }

    TArray<UMaterialInstanceDynamic*> AllMaterials;
    TArray<UMeshComponent*> MeshComponents;

    GetOwner()->GetComponents<UMeshComponent>(MeshComponents);

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
            }
            else
            {
                auto matInst = UMaterialInstanceDynamic::Create(Material, this);
                MeshComponent->SetMaterial(i, matInst);
                AllMaterials.Add(matInst);
            }
        }
    }

    StartFadeIn(AllMaterials);
}
void UMSPObjectFadeControllerComponent::StartFadeIn_Implementation(const TArray<UMaterialInstanceDynamic*>& Materials)
{
    if (!EnableFadeController)
    {
        for (auto Material : Materials)
        {
            Material->SetScalarParameterValue("Visibility", 1.0f);
        }
        return;
    }

    MaterialsToFade = Materials;
    CurrentFadeState = EObjectFadeState::FadingIn;
    CurrentFadeTime = 0.0f;

    for (auto Material : MaterialsToFade)
    {
        Material->SetScalarParameterValue("Visibility", 0.0f);
    }
}

void UMSPObjectFadeControllerComponent::StartFadeOut_Implementation(const TArray<UMaterialInstanceDynamic*>& Materials)
{
    if (!EnableFadeController)
    {
        for (auto Material : Materials)
        {
            Material->SetScalarParameterValue("Visibility", 0.0f);
        }
        GetOwner()->Destroy();
        return;
    }

    MaterialsToFade = Materials;
    CurrentFadeState = EObjectFadeState::FadingOut;
    CurrentFadeTime = 0.0f;

    for (auto Material : MaterialsToFade)
    {
        Material->SetScalarParameterValue("Visibility", 1.0f);
    }
}



void UMSPObjectFadeControllerComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{

    if (CurrentFadeState == EObjectFadeState::FadingIn)
    {
        CurrentFadeTime += DeltaTime;
        float Alpha = FMath::Clamp(CurrentFadeTime / FadeInTime, 0.0f, 1.0f);

        for (auto Material : MaterialsToFade)
        {
            Material->SetScalarParameterValue("Visibility", Alpha);
        }

        if (CurrentFadeTime >= FadeInTime)
        {
            CurrentFadeState = EObjectFadeState::Idle;
        }
    }
    else if (CurrentFadeState == EObjectFadeState::FadingOut)
    {
        CurrentFadeTime += DeltaTime;
        float Alpha = FMath::Clamp(CurrentFadeTime / FadeOutTime, 0.0f, 1.0f);

        for (auto Material : MaterialsToFade)
        {
            Material->SetScalarParameterValue("Visibility", 1.0f - Alpha);
        }

        if (CurrentFadeTime >= FadeOutTime)
        {
            CurrentFadeState = EObjectFadeState::Idle;
            GetOwner()->Destroy();
        }
    }
}

