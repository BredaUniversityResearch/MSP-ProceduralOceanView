#pragma once

#include "UObject/Object.h"
#include "ConfigMetaData.h"
#include "ConfigData.h"
#include "Config.generated.h"

UCLASS(BlueprintType)
// ReSharper disable CppInconsistentNaming, need to map with the json
class UConfig : public UObject
{
	GENERATED_BODY()

public:
	inline static constexpr float MetersToUnrealUnits = 100.0f;

	static UConfig* GetInstance();

	UPROPERTY(BlueprintReadOnly, Category = "Config")
		UConfigMetaData* metadata;
	UPROPERTY(BlueprintReadOnly, Category = "Config")
		UConfigData* datamodel;
};