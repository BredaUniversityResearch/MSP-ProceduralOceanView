#pragma once

#include "StringMap.generated.h"

UCLASS(BlueprintType)
class UStringMap : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Config|Common", DisplayName="Values")
		TMap<FString, FString> m_values;
};
