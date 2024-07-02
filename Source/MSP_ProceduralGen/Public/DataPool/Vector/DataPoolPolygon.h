#pragma once

#include "DataPoolVectorShape.h"
#include "DataPoolVectorArrayWrapper.h"
#include "DataPoolPolygon.generated.h"

USTRUCT(BlueprintType)
struct FDataPoolPolygon : public FDataPoolVectorShape
{
	GENERATED_BODY()

	FDataPoolPolygon() = default;

	FDataPoolPolygon(const FDataPoolVectorArrayWrapper& a_points, const TArray<FDataPoolVectorArrayWrapper>& a_gaps):
		FDataPoolVectorShape(),
		m_points(a_points),
		m_gaps(a_gaps)
	{
	}

	virtual void OnSetupFromConfig(const UVectorLayer* a_configLayer, const UVectorObject* a_configData) override
	{
		for (auto& gaps = a_configData->gaps; const auto& rawPoints : gaps)
		{
			m_gaps.Add(FDataPoolVectorArrayWrapper(rawPoints->GetValuesAsWorldVectorArray()));
		}
		m_points.m_vectorArray = a_configData->GetPointsAsWorldVector();
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data pool|Vector data sources|Polygons data source|Polygon", Meta = (DisplayName = "Points"))
	FDataPoolVectorArrayWrapper m_points;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data pool|Vector data sources|Polygons data source|Polygon", Meta = (DisplayName = "Gaps"))
	TArray<FDataPoolVectorArrayWrapper> m_gaps;
};
