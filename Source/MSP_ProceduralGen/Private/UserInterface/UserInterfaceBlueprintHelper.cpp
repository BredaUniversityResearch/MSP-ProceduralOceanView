#include "UserInterfaceBlueprintHelper.h"

#include "UObject/UnrealType.h" 
#include "UObject\UnrealTypePrivate.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Slate/SGameLayerManager.h"

TArray<FString> UUserInterfaceBlueprintHelper::GetScalabilityQualityTexts()
{
	Scalability::FQualityLevels levels = Scalability::GetQualityLevelCounts();

	int count = FMath::Min(TArray{ levels.AntiAliasingQuality, levels.EffectsQuality, levels.FoliageQuality, levels.GlobalIlluminationQuality, levels.ReflectionQuality, levels.PostProcessQuality, levels.ShadingQuality, levels.ShadowQuality, levels.TextureQuality, levels.ViewDistanceQuality });

	TArray<FString> texts = TArray<FString>();
	texts.Reserve(count);

	for (int i = 0; i < count; ++i)
	{
		FText text = Scalability::GetQualityLevelText(i, count);
		texts.Emplace(text.ToString());
	}

	return texts;
}

void UUserInterfaceBlueprintHelper::ZOrderChildFromPanelWidgetAsHighest(UPanelWidget* PanelWidget, UWidget* ChildWidget) {
	if (PanelWidget == nullptr || ChildWidget == nullptr)
	{
		return;
	}

	auto children = PanelWidget->GetAllChildren();

	int32 highestZOrder = MIN_int32;
	for (auto& Slot : children)
	{
		auto CanvasSlot = Cast<UCanvasPanelSlot>(Slot->Slot);
		if (CanvasSlot == nullptr)
		{
			continue;
		}
		auto zOrder = CanvasSlot->GetZOrder();
		if (zOrder > highestZOrder)
		{
			highestZOrder = zOrder;
		}
	}

	Cast<UCanvasPanelSlot>(ChildWidget->Slot)->SetZOrder(highestZOrder + 1);

}

bool UUserInterfaceBlueprintHelper::CanWidgetBeClicked(UObject* WorldContextObject, UPanelWidget* PanelWidget, UWidget* ChildWidget, const FVector2D& MousePosition)
{
	if (PanelWidget == nullptr || ChildWidget == nullptr)
	{
		return false;
	}

	UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(ChildWidget->Slot);
	if (slot == nullptr)
	{
		return false;
	}
	int32 childZOrder = slot->GetZOrder();

	for (auto& child : PanelWidget->GetAllChildren())
	{
		if (child == ChildWidget)
		{
			continue;
		}

		auto CanvasSlot = Cast<UCanvasPanelSlot>(child->Slot);
		if (CanvasSlot == nullptr)
		{
			continue;
		}

		auto zOrder = CanvasSlot->GetZOrder();
		if (zOrder < childZOrder)
		{
			continue;
		}

		auto childasuserwidget = Cast<UUserWidget>(child);
		if (childasuserwidget == nullptr)
		{
			continue;
		}

		auto rootcanvas = Cast<UCanvasPanel>(childasuserwidget->GetRootWidget());
		if (rootcanvas == nullptr)
		{
			continue;
		}

		auto windowedPanel = rootcanvas->GetChildAt(1);
		auto windowedPanelCanvasSlot = Cast<UCanvasPanelSlot>(windowedPanel->Slot);

		auto windowSize = windowedPanelCanvasSlot->GetSize();
		auto windowPos = windowedPanelCanvasSlot->GetPosition();
		if (IsPointInRect(MousePosition, windowPos, windowPos + windowSize))
		{
			return false;
		}
	}
	return true;
}

bool UUserInterfaceBlueprintHelper::IsPointInRect(const FVector2D& Point, const FVector2D& TopLeft, const FVector2D& BottomRight)
{
	return Point.X >= TopLeft.X && Point.X <= BottomRight.X && Point.Y >= TopLeft.Y && Point.Y <= BottomRight.Y;
}

TArray<FDisplayModeInterfaceInformation> UUserInterfaceBlueprintHelper::FilterDisplayModesByLayer(const TArray<FDisplayModeInterfaceInformation>& DisplayModes, const FString& UILayerGroup)
{
	TArray<FDisplayModeInterfaceInformation> result;

	for (const auto& displayMode : DisplayModes)
	{
		if (displayMode.UILayerGroup == UILayerGroup)
		{
			result.Emplace(displayMode);
		}
	}

	return result;
}

TArray<FDisplayModeInterfaceInformation> UUserInterfaceBlueprintHelper::FilterDisplayModesByCategory(const TArray<FDisplayModeInterfaceInformation>& DisplayModes, const EProceduralRulePackDisplayModeCategory& Category)
{
	TArray<FDisplayModeInterfaceInformation> result;

	for (const auto& displayMode : DisplayModes)
	{
		if (displayMode.Category == Category)
		{
			result.Emplace(displayMode);
		}
	}

	return result;
}

TArray<FDisplayModeInterfaceInformation> UUserInterfaceBlueprintHelper::SortDisplayModesByLayeringMode(const TArray<FDisplayModeInterfaceInformation>& DisplayModes)
{
	TArray<FDisplayModeInterfaceInformation> result;

	for (const auto& displayMode : DisplayModes)
	{
		if (displayMode.LayeringMode == EDisplayModeLayeringType::Exclusive)
		{
			result.Emplace(displayMode);
		}
	}

	for (const auto& displayMode : DisplayModes)
	{
		if (displayMode.LayeringMode == EDisplayModeLayeringType::Permissive)
		{
			result.Emplace(displayMode);
		}
	}

	for (const auto& displayMode : DisplayModes)
	{
		if (displayMode.LayeringMode == EDisplayModeLayeringType::Additive)
		{
			result.Emplace(displayMode);
		}
	}

	return result;
}

TArray<FString> UUserInterfaceBlueprintHelper::GetDisplayModeLayerNames(const TArray<FDisplayModeInterfaceInformation>& DisplayModes)
{
	TArray<FString> result;

	for (const auto& displayMode : DisplayModes)
	{
		if (!result.Contains(displayMode.UILayerGroup))
		{
			result.Emplace(displayMode.UILayerGroup);
		}
	}

	return result;
}
