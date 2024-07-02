#pragma once

#include "UObject/Object.h"
#include "ALayer.h"
#include "DataPool/DataPoolDataTypes.h"
#include "VectorLayer.generated.h"

class UVectorObject;

UCLASS(BlueprintType)
// ReSharper disable CppInconsistentNaming, need to map with the json
class UVectorLayer : public UALayer
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Vector layer", DisplayName="Array of data for vector layers")
        TArray<UVectorObject*> data;

    EDataPoolDataType GetDataTypeFromTags() const;
};
