#include "Config/Common/FloatArrayWrapper.h"

#include "Config/Config.h"

FVector2D UFloatArrayWrapper::GetRawValuesAsVector2D() const
{
	return FVector2D(
		m_values[0],
		m_values[1]
	);
}

FVector2D UFloatArrayWrapper::AsProjectedVector2D(const FTransform& a_transform) const
{
	const FVector v = AsProjectedVector(a_transform);
	return FVector2D(v.X, v.Y);
}

FVector UFloatArrayWrapper::AsProjectedVector(const FTransform& a_transform) const
{
	return a_transform.TransformVector(FVector(m_values[0], m_values[1], 0));
}

FVector2D UFloatArrayWrapper::AsWorldVector2D() const
{
	const auto& config = UConfig::GetInstance();
	const FVector2D& worldOffset = config->datamodel->GetWorldOffset();

	FTransform t;
	t.SetScale3D(FVector(UConfig::MetersToUnrealUnits, UConfig::MetersToUnrealUnits, UConfig::MetersToUnrealUnits));
	FVector2D v = AsProjectedVector2D(t);
	v -= FVector2D(worldOffset.X, worldOffset.Y);
	v.Y = -v.Y; //POV-671 - Y Axis seems flipped on all geometry compared to QGIS and MSP
	return v;
}

FVector UFloatArrayWrapper::AsWorldVector() const
{
	const auto& config = UConfig::GetInstance();
	const FVector2D& worldOffset = config->datamodel->GetWorldOffset();

	FTransform t;
	t.SetScale3D(FVector(UConfig::MetersToUnrealUnits, UConfig::MetersToUnrealUnits, UConfig::MetersToUnrealUnits));
	FVector v(AsProjectedVector(t));
	v -= FVector(worldOffset.X, worldOffset.Y, 0);
	v.Y = -v.Y; //POV-671 - Y Axis seems flipped on all geometry compared to QGIS and MSP
	return v;
}
