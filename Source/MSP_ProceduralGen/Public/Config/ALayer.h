#pragma once

#include "UObject/Object.h"
#include "LabelInfoConfig.h"
#include "Common/StringMap.h"
#include "ALayer.generated.h"

UCLASS(Abstract, BlueprintType)
// ReSharper disable CppInconsistentNaming, need to map with the json
class UALayer : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer", DisplayName="Layer name")
	FString name;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer", DisplayName="Layer tags")
	TArray<FName> tags;
	UPROPERTY(SkipSerialization) //We manually load this to avoid duplicate entries.
	TArray<UStringMap*> types;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer", DisplayName="Label info, for non-raster layers")
	ULabelInfoConfig* labelinfo;
};
