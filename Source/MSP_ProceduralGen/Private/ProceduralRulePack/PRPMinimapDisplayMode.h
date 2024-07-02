// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"
#include "ProceduralRulePackDisplayModeCategory.h"
#include "ProceduralRulePackDisplayMode.h"


#include "PRPMinimapDisplayMode.generated.h"

UENUM(BlueprintType)
enum class ELocatorPinMode : uint8
{
	Invisible,
	Local,
	World
};


/**
 *
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class UPRPMinimapDisplayMode : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void EnableDisplayMode(UDataPool* a_dataPool);

	UFUNCTION(BlueprintNativeEvent)
	void DisableDisplayMode(UDataPool* a_dataPool);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsCurrentlyEnabled() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FName GetDisplayModeName() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FString GetHumanReadableName() const;

	EDisplayModeLayeringType GetLayeringMode() const { return EDisplayModeLayeringType::Exclusive; }
	EProceduralRulePackDisplayModeCategory GetUICategory() const { return UICategory; }


	UFUNCTION(BlueprintCallable, BlueprintPure)
	AGameModeBase* PRPDisplayModeGetGameMode() const;

protected:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UGameInstance* PRPDisplayModeGetGameInstance() const;

	UFUNCTION(BlueprintCallable)
	UTextureRenderTarget2D* DrawMaterialToRenderTarget(UMaterialInterface* a_material, UTexture* a_input, const FBox2D& SimulatedBounds, const FBox2D& TextureCoveredBounds);

private:
	UPROPERTY(EditDefaultsOnly)
	FString HumanReadableName;

	UPROPERTY(EditDefaultsOnly, Category = "UI Information")
	EProceduralRulePackDisplayModeCategory UICategory;

	UPROPERTY(EditDefaultsOnly, Category = "UI Information")
	FString UILayerGroup;

	UPROPERTY(Transient)
	bool CurrentlyEnabled{ false };
};
