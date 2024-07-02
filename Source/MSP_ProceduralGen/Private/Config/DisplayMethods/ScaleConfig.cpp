#include "Config/DisplayMethods/ScaleConfig.h"
#include "Config/Config.h"

float UScaleConfig::GetWorldScaleMinValue() const
{
	return min_value * UConfig::MetersToUnrealUnits;
}

float UScaleConfig::GetWorldScaleMaxValue() const
{
	return max_value * UConfig::MetersToUnrealUnits;
}
