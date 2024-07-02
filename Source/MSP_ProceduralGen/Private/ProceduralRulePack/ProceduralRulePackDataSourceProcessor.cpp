#include "ProceduralRulePackDataSourceProcessor.h"

#include "ProceduralRulePackDisplayMode.h"

void UProceduralRulePackDataSourceProcessor::RegisterWithDataPool_Implementation(UDataPool* DataPool)
{
	TargetDataPool = DataPool;
}
