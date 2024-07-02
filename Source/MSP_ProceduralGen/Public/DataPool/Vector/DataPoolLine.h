#pragma once

#include "DataPoolVectorArrayWrapper.h"
#include "DataPoolVectorShape.h"
#include "DataPoolLine.generated.h"

USTRUCT(BlueprintType)
struct FDataPoolLine : public FDataPoolVectorShape
{
	GENERATED_BODY()

	FDataPoolLine() = default;

	explicit FDataPoolLine(const FDataPoolVectorArrayWrapper& a_points)
		: FDataPoolVectorShape(), m_points(a_points)
	{
	}

	virtual void OnSetupFromConfig(const UVectorLayer* a_configLayer, const UVectorObject* a_configData) override
	{
		m_points.m_vectorArray = a_configData->GetPointsAsWorldVector();
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Data pool|Vector data sources|Lines data source|Line", Meta = (DisplayName="Points"))
		FDataPoolVectorArrayWrapper m_points;
};
