#pragma once

#include "DataPoolVectorShape.h"
#include "DataPoolPoint.generated.h"

USTRUCT(BlueprintType)
struct FDataPoolPoint : public FDataPoolVectorShape
{
	GENERATED_BODY()

	FDataPoolPoint()
	    : FDataPoolVectorShape(), m_vector(FVector::ZeroVector)
	{
	}

	explicit FDataPoolPoint(const FVector& a_vector)
		: FDataPoolVectorShape(), m_vector(a_vector)
	{
	}

	virtual void OnSetupFromConfig(const UVectorLayer* a_configLayer, const UVectorObject* a_configData) override
	{
		m_vector = a_configData->GetPointsAsWorldVector()[0];
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Data pool|Vector data sources|Points data source|Point", Meta = (DisplayName="Point's vector"))
		FVector m_vector;
};
