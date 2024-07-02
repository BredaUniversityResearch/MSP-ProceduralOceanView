#pragma once

#include "DataPoolPoint.h"
#include "DataPoolPoints.generated.h"

USTRUCT(BlueprintType)
struct FDataPoolPoints
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Data pool|Vector data sources|Points data source", Meta = (DisplayName="Points"))
		TArray<FDataPoolPoint> m_points;
};