#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ArrayFunctions.generated.h"

UCLASS()
class MSP_PROCEDURALGEN_API UArrayFunctions : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Array Functions")
        static TArray<int32> InvertArray(const TArray<int32>& InputArray, int32 MaxIndex);
};
