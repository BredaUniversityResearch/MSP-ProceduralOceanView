#pragma once

#include "LabelInfoConfigContent.h"
#include "UObject/Object.h"
#include "LabelInfoConfig.generated.h"

UCLASS(BlueprintType)
class ULabelInfoConfig : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Label info", DisplayName="The colour of the label")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString colour;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Label info", DisplayName="List of label content")
		TArray<ULabelInfoConfigContent*> content;

	UFUNCTION(BlueprintCallable, Category = "Config|Data model|Layer|Label info", DisplayName="Get colour as FColor")
		FColor GetColourAsObject() const;
};
