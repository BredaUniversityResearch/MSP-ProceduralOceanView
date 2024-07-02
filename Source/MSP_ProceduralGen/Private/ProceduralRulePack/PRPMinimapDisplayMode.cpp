// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralRulePack/PRPMinimapDisplayMode.h"
#include "PRPMinimapDisplayMode.h"
#include "Engine/Canvas.h"

#include "Kismet/KismetRenderingLibrary.h"


void UPRPMinimapDisplayMode::EnableDisplayMode_Implementation(UDataPool* a_dataPool)
{
	check(!CurrentlyEnabled);
	CurrentlyEnabled = true;
}

void UPRPMinimapDisplayMode::DisableDisplayMode_Implementation(UDataPool* a_dataPool)
{
	check(CurrentlyEnabled);
	CurrentlyEnabled = false;
}

bool UPRPMinimapDisplayMode::IsCurrentlyEnabled() const
{
	return CurrentlyEnabled;
}

FName UPRPMinimapDisplayMode::GetDisplayModeName() const
{
	return GetFName();
}

FString UPRPMinimapDisplayMode::GetHumanReadableName() const
{
	return HumanReadableName;
}

AGameModeBase* UPRPMinimapDisplayMode::PRPDisplayModeGetGameMode() const
{
	return GetWorld()->GetAuthGameMode();
}

UGameInstance* UPRPMinimapDisplayMode::PRPDisplayModeGetGameInstance() const
{
	return GetWorld()->GetGameInstance();
}

UTextureRenderTarget2D* UPRPMinimapDisplayMode::DrawMaterialToRenderTarget(UMaterialInterface* a_material, UTexture* a_input, const FBox2D& SimulatedBounds, const FBox2D& TextureCoveredBounds)
{
	check(a_material);
	check(a_input);

	const FVector2D targetResolution = {256, 256};
	auto* displayTextureRenderTarget = NewObject<UTextureRenderTarget2D>(this);
	displayTextureRenderTarget->InitAutoFormat(targetResolution.X, targetResolution.Y);

	auto* renderMaterial = UMaterialInstanceDynamic::Create(a_material, this);
	renderMaterial->SetTextureParameterValue("TexInput", a_input);

	FVector2D displayAreaExtents = SimulatedBounds.GetSize(); 
	FVector2D projectedMin = (TextureCoveredBounds.Min - SimulatedBounds.Min) / displayAreaExtents;
	FVector2D projectedMax = FVector2D::One() + ((TextureCoveredBounds.Max - SimulatedBounds.Max) / displayAreaExtents);
	FVector2D projectedPosition = projectedMin;
	FVector2D projectedSize = projectedMax - projectedMin;

	UCanvas* targetCanvas = nullptr;
	FVector2D size{};
	FDrawToRenderTargetContext context{};
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(GetWorld(), displayTextureRenderTarget, targetCanvas, size, context);

	targetCanvas->K2_DrawMaterial(renderMaterial, projectedPosition * targetResolution, projectedSize * targetResolution, FVector2D::Zero(), FVector2D::One());

	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetWorld(), context);

	return displayTextureRenderTarget;
}
