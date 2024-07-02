#pragma once
#include "ProceduralRulePackDisplayMode.h"
#include "PRPMinimapDisplayMode.h"

#include "ProceduralRulePackManager.generated.h"

class UDataPool;
class UProceduralRulePackDataSourceProcessor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPRPDisplayModeToggled, FName, ToggledDisplayMode, bool, CurrentlyEnabled);

USTRUCT(BlueprintType)
struct FDisplayModeInterfaceInformation
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FName DisplayModeName;	//Internal ID to toggle display mode on and off.
	UPROPERTY(BlueprintReadOnly)
	FString DisplayName;	//Human readable name to display
	UPROPERTY(BlueprintReadOnly)
	FString UILayerGroup;		//Category of the DisplayMode
	UPROPERTY(BlueprintReadOnly)
	EProceduralRulePackDisplayModeCategory Category = EProceduralRulePackDisplayModeCategory::Biology;	//Category of the DisplayMode
	UPROPERTY(BlueprintReadOnly)
	bool CurrentlyEnabled = false;
	UPROPERTY(BlueprintReadOnly)
	EDisplayModeLayeringType LayeringMode = EDisplayModeLayeringType::Exclusive;
};

UCLASS(BlueprintType)
class UProceduralRulePackManager: public UObject, public FTickableGameObject
{
	GENERATED_BODY()

	struct FDisplayModeCategoryState
	{
		UProceduralRulePackDisplayMode* EnabledPermissiveDisplayMode{nullptr};
		TArray<UProceduralRulePackDisplayMode*> EnabledAdditiveModes{};
	};

	struct FMinimapDisplayModeCategoryState
	{
		UPRPMinimapDisplayMode* EnabledPermissiveDisplayMode{nullptr};
		TArray<UPRPMinimapDisplayMode*> EnabledAdditiveModes{};
	};

public:
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UProceduralRulePackManager, STATGROUP_Tickables)};

	void SetTargetDataPool(UDataPool& DataPool);

	UFUNCTION(BlueprintCallable)
	void EnableDisplayMode(const FName& DisplayModeName);
	UFUNCTION(BlueprintCallable)
	void DisableDisplayMode(const FName& DisplayModeName);

	UFUNCTION(BlueprintCallable)
	void EnableMinimapDisplayMode(const FName& DisplayModeName);
	UFUNCTION(BlueprintCallable)
	void DisableMinimapDisplayMode(const FName& DisplayModeName);

	UFUNCTION(BlueprintCallable)
	TArray<FDisplayModeInterfaceInformation> GetDisplayModesInformation() const;

	UFUNCTION(BlueprintCallable)
	TArray<FDisplayModeInterfaceInformation> GetMinimapDisplayModesInformation() const;

	void EnableDefaultDisplayModes();

	UPROPERTY(BlueprintAssignable)
	FPRPDisplayModeToggled OnDisplayModeToggled;

	UPROPERTY(BlueprintAssignable)
	FPRPDisplayModeToggled OnMinimapDisplayModeToggled;

private:
	UFUNCTION()
	void InitializeDataSourceProcessors(UDataPool* DataPool);

	UPROPERTY(EditAnywhere)
	double DisplayModeDisableEnableDelaySeconds = 1.5;

	UPROPERTY(Transient)
	UDataPool* TargetDataPool;

	TMap<FString, FDisplayModeCategoryState> DisplayModeCategoryStates;
	TMap<FString, FMinimapDisplayModeCategoryState> MinimapDisplayModeCategoryStates;

	UPROPERTY(Transient)
	TArray<UProceduralRulePackDataSourceProcessor*> DataSourceInstances{};
	UPROPERTY(Transient)
	TMap<FName, UProceduralRulePackDisplayMode*> DisplayModeInstancesByName{};
	//UPROPERTY(Transient)
	TMap<FNameEntryId, TStrongObjectPtr<UPRPMinimapDisplayMode>> MinimapDisplayModeInstancesByName{};

};

