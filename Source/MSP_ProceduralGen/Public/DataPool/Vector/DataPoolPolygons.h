#pragma once

#include "DataPoolPolygon.h"
#include "DataPoolPolygons.generated.h"

USTRUCT(BlueprintType)
struct FDataPoolPolygons
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Data pool|Vector data sources|Polygons data source", Meta = (DisplayName="Polygons"))
		TArray<FDataPoolPolygon> m_polygons;
};
