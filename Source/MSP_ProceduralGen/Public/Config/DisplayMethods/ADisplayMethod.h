#pragma once

#include "../Common/FloatArrayWrapper.h"
#include "../Common/StringMap.h"
#include "UObject/Object.h"
#include "ADisplayMethod.generated.h"

UCLASS(Abstract, BlueprintType)
// ReSharper disable CppInconsistentNaming
class UADisplayMethod : public UObject
{
    GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Config|Data model|Layer|Display method")
	FString GetMethod()
	{
		return method;
	}
	UPROPERTY(SkipSerialization, BlueprintReadOnly, Category = "Config|Data model|Layer|Display method")
		TArray<UStringMap*> stages;
	UPROPERTY(SkipSerialization, BlueprintReadOnly, Category = "Config|Data model|Layer|Display method")
		TArray<UFloatArrayWrapper*> stage_distribution;

protected:
	UPROPERTY(SkipSerialization)
		FString method;
};
