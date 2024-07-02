#pragma once

#include "DataPoolVectorWrapper.generated.h"

USTRUCT(BlueprintType)
struct FDataPoolVectorWrapper
{
	GENERATED_BODY()

	explicit FDataPoolVectorWrapper()
		: m_vector(FVector::ZeroVector)
	{
	}

	explicit FDataPoolVectorWrapper(const FVector& a_vector)
		: m_vector(a_vector)
	{
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Data pool|Vector data sources", Meta = (DisplayName="Vector"))
		FVector m_vector;
};