#include "Config/LabelInfoConfig.h"

FColor ULabelInfoConfig::GetColourAsObject() const
{
	return FColor::FromHex(colour);
}
