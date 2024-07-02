#pragma once

#include "ProceduralRulePackDisplayModeCategory.generated.h"

UENUM(BlueprintType)
enum class EProceduralRulePackDisplayModeCategory: uint8
{
	Energy,
	Biology,
	Vessels,
	Environment,
	Extraction,
	Protection,
	Management
};
