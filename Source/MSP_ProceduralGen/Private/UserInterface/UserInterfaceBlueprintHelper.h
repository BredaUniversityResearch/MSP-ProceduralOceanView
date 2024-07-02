#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Components/PanelWidget.h"
#include "../ProceduralRulePack/ProceduralRulePackManager.h"

#include "UserInterfaceBlueprintHelper.generated.h"

UCLASS()
class UUserInterfaceBlueprintHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static TArray<FString> GetScalabilityQualityTexts();

	UFUNCTION(BlueprintCallable)
	static void ZOrderChildFromPanelWidgetAsHighest(UPanelWidget* PanelWidget, UWidget* ChildWidget);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=( WorldContext="WorldContextObject" ))
	static bool CanWidgetBeClicked(UObject* WorldContextObject, UPanelWidget* PanelWidget, UWidget* ChildWidget, const FVector2D& MousePosition);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static bool IsPointInRect(const FVector2D& Point, const FVector2D& TopLeft, const FVector2D& BottomRight);


	UFUNCTION(BlueprintCallable )
	static TArray<FDisplayModeInterfaceInformation> FilterDisplayModesByLayer(const TArray<FDisplayModeInterfaceInformation>& DisplayModes, const FString& UILayerGroup);

	UFUNCTION(BlueprintCallable)
	static TArray<FDisplayModeInterfaceInformation> FilterDisplayModesByCategory(const TArray<FDisplayModeInterfaceInformation>& DisplayModes, const EProceduralRulePackDisplayModeCategory& Category);

	UFUNCTION(BlueprintCallable)
	static TArray<FDisplayModeInterfaceInformation> SortDisplayModesByLayeringMode(const TArray<FDisplayModeInterfaceInformation>& DisplayModes); 

	UFUNCTION(BlueprintCallable)
	static TArray<FString> GetDisplayModeLayerNames(const TArray<FDisplayModeInterfaceInformation>& DisplayModes);



};