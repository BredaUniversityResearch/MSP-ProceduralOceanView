#pragma once

#include "DataSourceTypeMapping.generated.h"

USTRUCT(BlueprintType)
// ReSharper disable CppInconsistentNaming, need to map with the json
struct FDataSourceTypeMapping
{
	GENERATED_BODY()

	UPROPERTY()
	FName name;
	UPROPERTY()
	FString material;
};
