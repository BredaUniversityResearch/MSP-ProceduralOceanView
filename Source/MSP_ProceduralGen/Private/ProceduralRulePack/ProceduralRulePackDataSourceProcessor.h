#pragma once
#include "ProceduralRulePackManager.h"


#include "ProceduralRulePackDataSourceProcessor.generated.h"

class UDataPool;
class UProceduralRulePackDisplayMode;

UCLASS(Blueprintable, BlueprintType, Abstract)
class MSP_PROCEDURALGEN_API UProceduralRulePackDataSourceProcessor : public UObject
{
	GENERATED_BODY() 

public:
	UFUNCTION(BlueprintNativeEvent)
	void RegisterWithDataPool(UDataPool* DataPool);

protected:
	UPROPERTY(BlueprintReadOnly)
	UDataPool* TargetDataPool{nullptr};	
};