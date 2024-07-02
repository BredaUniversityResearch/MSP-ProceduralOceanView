#include "ProceduralRulePackManager.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "ProceduralRulePackDataSourceProcessor.h"
#include "ProceduralRulePackDisplayMode.h"
#include "ProceduralRulePackManifest.h"
#include "DataPool/DataPool.h"
#include "MSP_ProceduralGen/MSP_ProceduralGen.h"

namespace
{
	template<typename TTargetType>
	TArray<TTargetType*> FindAllAssetsOfType()
	{
		TArray<TTargetType*> result = {};
		UClass* targetClass = TTargetType::StaticClass();

		TArray<FAssetData> AssetData;
		FString classPathName = GetPathNameSafe(targetClass);

		IAssetRegistry::Get()->GetAssetsByClass(FTopLevelAssetPath(classPathName), AssetData, true);

		for (int i = 0; i < AssetData.Num(); i++) {
			TTargetType* Object = Cast<TTargetType>(AssetData[i].GetAsset());
			ensure(Object);
			result.Add(Object);
		}
		return result;
	}
}

void UProceduralRulePackManager::Tick(float DeltaTime)
{
	for (auto it : DisplayModeInstancesByName)
	{
		double targetEnableTime = it.Value->GetDisplayModePendingEnableTime();
		if (targetEnableTime >= 0.0 && targetEnableTime < GetWorld()->TimeSeconds)
		{
			it.Value->EnableDisplayMode(TargetDataPool);
		}
	}
}

void UProceduralRulePackManager::SetTargetDataPool(UDataPool& DataPool)
{
	TargetDataPool = &DataPool;
	FOnDataPoolSetupFinishedMulticastDelegate::FDelegate setupFinishedDelegate;
	setupFinishedDelegate.BindDynamic(this, &UProceduralRulePackManager::InitializeDataSourceProcessors);
	TargetDataPool->RegisterDataPoolSetupFinishedListener(setupFinishedDelegate);
}

void UProceduralRulePackManager::EnableDisplayMode(const FName& DisplayModeName)
{
	UProceduralRulePackDisplayMode** displayModeIt = DisplayModeInstancesByName.Find(DisplayModeName);
	if (displayModeIt != nullptr)
	{
		UProceduralRulePackDisplayMode* displayMode = *displayModeIt;
		if (displayMode->IsCurrentlyOrPendingEnabled())
		{
			return; //Don't enable display mode when its already on.
		}

		FString category = displayMode->GetUILayerGroup();
		FDisplayModeCategoryState* categoryState = DisplayModeCategoryStates.Find(category);
		if (categoryState == nullptr)
		{
			categoryState = &DisplayModeCategoryStates.Emplace(category);
		}

		EDisplayModeLayeringType layeringType = displayMode->GetLayeringMode();
		if (layeringType == EDisplayModeLayeringType::Additive)
		{
			bool hasConflictedModes = false;
			if (categoryState->EnabledPermissiveDisplayMode != nullptr && categoryState->EnabledPermissiveDisplayMode->GetLayeringMode() == EDisplayModeLayeringType::Exclusive)
			{
				categoryState->EnabledPermissiveDisplayMode->DisableDisplayMode(TargetDataPool);
				OnDisplayModeToggled.Broadcast(categoryState->EnabledPermissiveDisplayMode->GetDisplayModeName(), false);
				categoryState->EnabledPermissiveDisplayMode = nullptr;
				hasConflictedModes = true;
			}

			categoryState->EnabledAdditiveModes.Add(displayMode);
			displayMode->SetDisplayModePendingEnable(GetWorld()->TimeSeconds + ((hasConflictedModes)? DisplayModeDisableEnableDelaySeconds : 0.0));
			OnDisplayModeToggled.Broadcast(displayMode->GetDisplayModeName(), true);
		}
		else if (layeringType == EDisplayModeLayeringType::Permissive || layeringType == EDisplayModeLayeringType::Exclusive)
		{
			bool hasConflictedModes = false;
			if (categoryState->EnabledPermissiveDisplayMode != nullptr)
			{
				categoryState->EnabledPermissiveDisplayMode->DisableDisplayMode(TargetDataPool);
				OnDisplayModeToggled.Broadcast(categoryState->EnabledPermissiveDisplayMode->GetDisplayModeName(), false);
				hasConflictedModes = true;
			}
			categoryState->EnabledPermissiveDisplayMode = displayMode;

			if (layeringType == EDisplayModeLayeringType::Exclusive)
			{
				for (const auto& it : categoryState->EnabledAdditiveModes)
				{
					it->DisableDisplayMode(TargetDataPool);
					OnDisplayModeToggled.Broadcast(it->GetDisplayModeName(), false);
					hasConflictedModes = true;
				}
				categoryState->EnabledAdditiveModes.Empty();
			}

			categoryState->EnabledPermissiveDisplayMode->SetDisplayModePendingEnable(GetWorld()->TimeSeconds + ((hasConflictedModes)? DisplayModeDisableEnableDelaySeconds : 0.0));
			OnDisplayModeToggled.Broadcast(categoryState->EnabledPermissiveDisplayMode->GetDisplayModeName(), true);
		}
		else
		{
			checkNoEntry()
		}
	}
	else
	{
		UE_LOG(LogMspProceduralGen, Warning, TEXT("Tried enabling display mode (%s) which was not found"), *DisplayModeName.ToString());
	}
}

void UProceduralRulePackManager::DisableDisplayMode(const FName& DisplayModeName)
{
	UProceduralRulePackDisplayMode** displayModeIt = DisplayModeInstancesByName.Find(DisplayModeName);
	if (displayModeIt != nullptr)
	{
		UProceduralRulePackDisplayMode* displayMode = *displayModeIt;
		if (!displayMode->IsCurrentlyOrPendingEnabled())
		{
			return; //Cannot disable something that is not on 
		}

		FDisplayModeCategoryState* categoryState = DisplayModeCategoryStates.Find(displayMode->GetUILayerGroup());
		if (categoryState != nullptr)
		{
			if (displayMode->GetLayeringMode() == EDisplayModeLayeringType::Additive)
			{
				int removedCount = categoryState->EnabledAdditiveModes.RemoveSwap(displayMode);
				ensure(removedCount == 1);
			}
			else
			{
				ensure(categoryState->EnabledPermissiveDisplayMode == displayMode);
				categoryState->EnabledPermissiveDisplayMode = nullptr;

			}
			displayMode->DisableDisplayMode(TargetDataPool);
			OnDisplayModeToggled.Broadcast(DisplayModeName, false);
		}
	}
}

void UProceduralRulePackManager::EnableMinimapDisplayMode(const FName& DisplayModeName)
{

	TStrongObjectPtr<UPRPMinimapDisplayMode>* displayModeIt = MinimapDisplayModeInstancesByName.Find(DisplayModeName.GetComparisonIndex());
	if (displayModeIt == nullptr)
	{
		UE_LOG(LogMspProceduralGen, Warning, TEXT("Tried enabling minimap display mode (%s) which was not found"), *DisplayModeName.ToString());
		return;
	}

	UPRPMinimapDisplayMode* displayMode = displayModeIt->Get();
	if (displayMode->IsCurrentlyEnabled())
	{
		return; //Don't enable display mode when its already on.
	}

	// for now we only support one minimap display mode at a time
	// but we use the same category system as the regular display modes for future proofing

	FString category = "Minimap";
	auto* categoryState = MinimapDisplayModeCategoryStates.Find(category);
	if (categoryState == nullptr)
	{
		categoryState = &MinimapDisplayModeCategoryStates.Emplace(category);
	}

	EDisplayModeLayeringType layeringType = EDisplayModeLayeringType::Exclusive;
	if (layeringType == EDisplayModeLayeringType::Additive)
	{
		checkNoEntry()
	}
	else if (layeringType == EDisplayModeLayeringType::Permissive || layeringType == EDisplayModeLayeringType::Exclusive)
	{

		if (categoryState->EnabledPermissiveDisplayMode == displayMode)
		{
			return; //Don't enable display mode when its already on.
		}

		if (categoryState->EnabledPermissiveDisplayMode != nullptr)
		{
			categoryState->EnabledPermissiveDisplayMode->DisableDisplayMode(TargetDataPool);
			OnMinimapDisplayModeToggled.Broadcast(categoryState->EnabledPermissiveDisplayMode->GetDisplayModeName(), false);
		}
		categoryState->EnabledPermissiveDisplayMode = displayMode;

		if (layeringType == EDisplayModeLayeringType::Exclusive)
		{
			for (const auto& it : categoryState->EnabledAdditiveModes)
			{
				it->DisableDisplayMode(TargetDataPool);
				OnMinimapDisplayModeToggled.Broadcast(it->GetDisplayModeName(), false);
			}
			categoryState->EnabledAdditiveModes.Empty();
		}

		categoryState->EnabledPermissiveDisplayMode->EnableDisplayMode(TargetDataPool);
		OnMinimapDisplayModeToggled.Broadcast(categoryState->EnabledPermissiveDisplayMode->GetDisplayModeName(), true);
	}
	else
	{
		checkNoEntry()
	}
}

void UProceduralRulePackManager::DisableMinimapDisplayMode(const FName& DisplayModeName)
{
	TStrongObjectPtr<UPRPMinimapDisplayMode>* displayModeIt = MinimapDisplayModeInstancesByName.Find(DisplayModeName.GetComparisonIndex());
	if (displayModeIt == nullptr)
	{
		UE_LOG(LogMspProceduralGen, Warning, TEXT("Tried disabling minimap display mode (%s) which was not found"), *DisplayModeName.ToString());
		return;
	}

	UPRPMinimapDisplayMode* displayMode = displayModeIt->Get();
	if (!displayMode->IsCurrentlyEnabled())
	{
		return; //Cannot disable something that is not on 
	}

	auto* categoryState = MinimapDisplayModeCategoryStates.Find("Minimap"); // for now we only support one minimap display mode at a time
	if (categoryState != nullptr)
	{
		if (displayMode->GetLayeringMode() == EDisplayModeLayeringType::Additive)
		{
			checkNoEntry()
		}
		else
		{
			ensure(categoryState->EnabledPermissiveDisplayMode == displayMode);
			categoryState->EnabledPermissiveDisplayMode = nullptr;

		}
		displayMode->DisableDisplayMode(TargetDataPool);
		OnMinimapDisplayModeToggled.Broadcast(DisplayModeName, false);
	}

}

TArray<FDisplayModeInterfaceInformation> UProceduralRulePackManager::GetDisplayModesInformation() const
{
	TArray<FDisplayModeInterfaceInformation> result;
	for (const auto& kvp : DisplayModeInstancesByName)
	{
		FDisplayModeInterfaceInformation info;
		info.DisplayModeName = kvp.Value->GetDisplayModeName();
		info.DisplayName = kvp.Value->GetHumanReadableName();
		info.UILayerGroup = kvp.Value->GetUILayerGroup();
		info.Category = kvp.Value->GetUICategory();
		info.CurrentlyEnabled = kvp.Value->IsCurrentlyOrPendingEnabled();
		info.LayeringMode = kvp.Value->GetLayeringMode();
		result.Emplace(MoveTemp(info));
	}
	return result;
}

TArray<FDisplayModeInterfaceInformation> UProceduralRulePackManager::GetMinimapDisplayModesInformation() const
{
	TArray<FDisplayModeInterfaceInformation> result;
	for (const auto& kvp : MinimapDisplayModeInstancesByName)
	{
		FDisplayModeInterfaceInformation info;
		info.DisplayModeName = kvp.Value->GetDisplayModeName();
		info.DisplayName = kvp.Value->GetHumanReadableName();
		info.UILayerGroup = "Minimap"; // for now we only support one minimap display mode at a time
		info.Category = kvp.Value->GetUICategory();
		info.CurrentlyEnabled = kvp.Value->IsCurrentlyEnabled();
		result.Emplace(MoveTemp(info));
	}
	return result;
}

void UProceduralRulePackManager::InitializeDataSourceProcessors(UDataPool* DataPool)
{
	ensure(TargetDataPool != nullptr);

	TArray<UProceduralRulePackManifest*> allManifests = FindAllAssetsOfType<UProceduralRulePackManifest>();
	for (const UProceduralRulePackManifest* manifest : allManifests)
	{
		for (auto definition : manifest->Processors)
		{
			UProceduralRulePackDataSourceProcessor* dataSourceProcessor = NewObject<UProceduralRulePackDataSourceProcessor>(this, definition);

			dataSourceProcessor->RegisterWithDataPool(TargetDataPool);
			DataSourceInstances.Add(dataSourceProcessor);
		}

		for (auto definition : manifest->DisplayModes)
		{
			UProceduralRulePackDisplayMode* displayMode = NewObject<UProceduralRulePackDisplayMode>(this, definition);
			if (displayMode->HasRequiredDataAvailable(DataPool))
			{
				DisplayModeInstancesByName.Emplace(displayMode->GetDisplayModeName(), displayMode);
			}
		}

		for (auto definition : manifest->MinimapDisplayModes)
		{
			UPRPMinimapDisplayMode* displayMode = NewObject<UPRPMinimapDisplayMode>(this, definition);
			MinimapDisplayModeInstancesByName.Emplace(displayMode->GetDisplayModeName().GetComparisonIndex(), TStrongObjectPtr(displayMode));
		}
	}

	EnableDefaultDisplayModes();
}

void UProceduralRulePackManager::EnableDefaultDisplayModes()
{
	for (const auto& displayMode : DisplayModeInstancesByName)
	{
		if (displayMode.Value->ShouldBeEnabledByDefault())
		{
			EnableDisplayMode(displayMode.Value->GetDisplayModeName());
		}
	}
}
