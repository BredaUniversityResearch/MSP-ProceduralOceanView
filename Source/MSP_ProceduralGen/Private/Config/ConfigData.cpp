#include "Config/ConfigData.h"
#include "Config/Config.h"

FVector2D UConfigData::GetCoordinate0AsWorldVector2D() const
{
	const FVector2D& worldOffset = GetWorldOffset();
	return FVector2D(
		coordinate0[0] * UConfig::MetersToUnrealUnits - worldOffset.X,
		coordinate0[1] * UConfig::MetersToUnrealUnits - worldOffset.Y
	);
}

FVector2D UConfigData::GetCoordinate1AsWorldVector2D() const
{
	const FVector2D& worldOffset = GetWorldOffset();
	return FVector2D(
		coordinate1[0] * UConfig::MetersToUnrealUnits - worldOffset.X,
		coordinate1[1] * UConfig::MetersToUnrealUnits - worldOffset.Y
	);
}

FVector2D UConfigData::GetWorldOffset() const
{
	return GetOffset() * UConfig::MetersToUnrealUnits;
}

FVector2D UConfigData::GetOffset() const
{
	return FVector2d(coordinate0[0], coordinate0[1]) + FVector2d(coordinate1[0]-coordinate0[0], coordinate1[1]-coordinate0[1]) * 0.5f;
}
