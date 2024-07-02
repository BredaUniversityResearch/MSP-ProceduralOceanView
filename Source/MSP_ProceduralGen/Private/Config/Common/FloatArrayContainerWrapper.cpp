#include "Config/Common/FloatArrayContainerWrapper.h"

TArray<FVector2D> UFloatArrayContainerWrapper::AsProjectedVector2DArray(const FTransform& a_transform) const
{
	TArray<FVector2D> result;
	for (const auto& point : m_values)
	{
		result.Add(point->AsProjectedVector2D(a_transform));
	}
	return result;
}

TArray<FVector> UFloatArrayContainerWrapper::AsProjectedVectorArray(const FTransform& a_transform) const
{
	return AsProjectedVectorArray(m_values, a_transform);
}

TArray<FVector> UFloatArrayContainerWrapper::AsProjectedVectorArray(const TArray<UFloatArrayWrapper*>& a_floatArrayContainer, const FTransform& a_transform)
{
	TArray<FVector> result;
	for (const auto& point : a_floatArrayContainer)
	{
		result.Add(point->AsProjectedVector(a_transform));
	}
	return result;
}

TArray<FVector2D> UFloatArrayContainerWrapper::GetRawValuesAsVector2DArray() const
{
	TArray<FVector2D> result;
	for (const auto& point : m_values)
	{
		result.Add(point->GetRawValuesAsVector2D());
	}
	return result;
}

TArray<FVector2D> UFloatArrayContainerWrapper::AsWorldVector2DArray(const TArray<UFloatArrayWrapper*>& a_floatArrayContainer)
{
	TArray<FVector2D> result;
	for (const auto& point : a_floatArrayContainer)
	{
		result.Add(point->AsWorldVector2D());
	}
	return result;
}

TArray<FVector> UFloatArrayContainerWrapper::AsWorldVectorArray(const TArray<UFloatArrayWrapper*>& a_floatArrayContainer)
{
	TArray<FVector> result;
	for (const auto& point : a_floatArrayContainer)
	{
		result.Add(point->AsWorldVector());
	}
	return result;
}

TArray<FVector2D> UFloatArrayContainerWrapper::GetValuesAsWorldVector2DArray() const
{
	return AsWorldVector2DArray(m_values);
}

TArray<FVector> UFloatArrayContainerWrapper::GetValuesAsWorldVectorArray() const
{
	return AsWorldVectorArray(m_values);
}
