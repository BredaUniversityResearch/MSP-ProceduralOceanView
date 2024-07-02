#pragma once

#include "Engine/DataAsset.h"
#include "ProceduralRulePackDisplayModeCategory.h"

#include "ProceduralRulePackDisplayMode.generated.h"

class UDataPool;
class UProceduralRulePackDataSourceProcessor;
class AProceduralRulePackActor;

UENUM(BlueprintType)
enum class EDisplayModeLayeringType: uint8
{
	//This is the only display mode that can apply to the target layer. Any other display modes will be suppressed.
	Exclusive,
	//Only a singular permissive mode may be active on a layer, there may be several additive layers. 
	Permissive,
	//Just layer on top of the rest.
	Additive
};

UCLASS(Blueprintable, BlueprintType, Abstract)
class UProceduralRulePackDisplayMode : public UObject
{
	GENERATED_BODY()

public:
	void SetDisplayModePendingEnable(double a_enableTimePoint);
	double GetDisplayModePendingEnableTime() const;

	UFUNCTION(BlueprintNativeEvent)
	void EnableDisplayMode(UDataPool* a_dataPool);
	UFUNCTION(BlueprintNativeEvent)
	void DisableDisplayMode(UDataPool* a_dataPool);

	EDisplayModeLayeringType GetLayeringMode() const { return LayeringMode; }
	FString GetUILayerGroup() const { return UILayerGroup; }
	EProceduralRulePackDisplayModeCategory GetUICategory() const { return UICategory; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsCurrentlyOrPendingEnabled() const;

	UFUNCTION(BlueprintNativeEvent)
	bool ShouldBeEnabledByDefault() const;
	UFUNCTION(BlueprintNativeEvent)
	bool HasRequiredDataAvailable(UDataPool* TargetDataPool) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FName GetDisplayModeName() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FString GetHumanReadableName() const;

	UFUNCTION(BlueprintCallable)
	AProceduralRulePackActor* SpawnPRPActor(TSubclassOf<AProceduralRulePackActor> ActorToSpawn, FVector Positon );

private:
	UPROPERTY(EditDefaultsOnly)
	EDisplayModeLayeringType LayeringMode;
	UPROPERTY(EditDefaultsOnly, Category = "UI Information") //Name displayed in UI
	FString HumanReadableName; 
	UPROPERTY(EditDefaultsOnly, Category = "UI Information") //Note PdG: Also used to resolve layering modes in the PRP Manager
	FString UILayerGroup;
	UPROPERTY(EditDefaultsOnly, Category = "UI Information")
	EProceduralRulePackDisplayModeCategory UICategory;

	UPROPERTY(EditDefaultsOnly)
	bool EnabledByDefault{false};

	UPROPERTY(Transient)
	bool CurrentlyEnabled{false};
	UPROPERTY(Transient)
	double PendingEnableTimePoint{-1.0};
};

