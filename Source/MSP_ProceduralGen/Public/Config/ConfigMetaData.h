#pragma once

#include "UObject/Object.h"
#include "ConfigMetaData.generated.h"

UCLASS(BlueprintType)
// ReSharper disable CppInconsistentNaming, need to map with the json
class UConfigMetaData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Config|Meta data")
		FString date_modified;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Meta data")
		FString data_model_hash;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Meta data")
		int32 errors;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Meta data")
		FString editor_version;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Meta data")
		FString region;
};
