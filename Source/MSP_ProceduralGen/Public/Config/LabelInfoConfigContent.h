#pragma once

#include "UObject/Object.h"
#include "LabelInfoConfigContent.generated.h"

UCLASS(BlueprintType)
class ULabelInfoConfigContent : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Label info|Content", DisplayName="The text of the label content")
		FString text;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Label info|Content", DisplayName="The name of the property to reference")
		FString ref_name;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Label info|Content", DisplayName="The target of the reference")
		FString ref_target;
};
