#pragma once

#include "ProceduralRulePackDataSourceProcessor.h"
#include "Engine/DataAsset.h"

#include "ProceduralRulePackManifest.generated.h"

UCLASS()
class UProceduralRulePackManifest: public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UProceduralRulePackDataSourceProcessor>> Processors;
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UProceduralRulePackDisplayMode>> DisplayModes;
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UPRPMinimapDisplayMode>> MinimapDisplayModes;
};
