#pragma once

#include "DataPoolVectorArrayWrapper.generated.h"

USTRUCT(BlueprintType)
struct FDataPoolVectorArrayWrapper
{
	GENERATED_BODY()

	FDataPoolVectorArrayWrapper() = default;

	explicit FDataPoolVectorArrayWrapper(const TArray<FVector>& a_vectorArray)
		: m_vectorArray(a_vectorArray)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Data pool|Vector data sources", Meta = (DisplayName="Array of vectors"))
		TArray<FVector> m_vectorArray;
};