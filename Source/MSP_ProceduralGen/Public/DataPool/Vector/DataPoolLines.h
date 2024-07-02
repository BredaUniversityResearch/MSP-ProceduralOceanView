#pragma once

#include "DataPoolLine.h"
#include "DataPoolLines.generated.h"

USTRUCT(BlueprintType)
struct FDataPoolLines
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Data pool|Vector data sources|Lines data source", Meta = (DisplayName="Lines"))
		TArray<FDataPoolLine> m_lines;
};