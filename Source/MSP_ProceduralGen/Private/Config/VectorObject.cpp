#include "Config/VectorObject.h"

#include "Config/Config.h"

TArray<FVector2D> UVectorObject::GetPointsAsWorldVector2D() const
{
	return UFloatArrayContainerWrapper::AsWorldVector2DArray(points);
}

TArray<FVector> UVectorObject::GetPointsAsWorldVector() const
{
	return UFloatArrayContainerWrapper::AsWorldVectorArray(points);
}

const FString& UVectorObject::GetMetadataValue(const FString& a_key) const
{
	return metadata->m_values[a_key];
}

float UVectorObject::GetMetadataValueAsFloat(const FString& a_key) const
{
	return FCString::Atof(*GetMetadataValue(a_key));
}

int32 UVectorObject::GetMetadataValueAsInt(const FString& a_key) const
{
	return FCString::Atoi(*GetMetadataValue(a_key));
}

bool UVectorObject::GetMetadataValueAsBool(const FString& a_key) const
{
	return GetMetadataValue(a_key) == "true";
}

TArray<FVector> UVectorObject::ResampleLinePoints(const float a_segmentLength = DEFAULT_SEGMENT_LENGTH) const
{
	float segmentLength = a_segmentLength;
	if (segmentLength < FLT_EPSILON) {
		UE_LOG(LogTemp, Error, TEXT("Encountered invalid segment length value: %f. Using default value: %f"), a_segmentLength, segmentLength);
		segmentLength = DEFAULT_SEGMENT_LENGTH;
	}
	TArray<FVector> result;
	const TArray linePoints3D(GetPointsAsWorldVector());
	for (int32 i = 0; i < linePoints3D.Num() - 1; i++)
	{
		FVector startPoint = linePoints3D[i];
		FVector endPoint = linePoints3D[i + 1];

		float distance = FVector::Dist(startPoint, endPoint);
		int32 samples = FMath::CeilToInt(distance / segmentLength);

		result.Add(startPoint);

		for (int32 j = 1; j < samples; j++)
		{
			FVector resampledPoint = FMath::Lerp(startPoint, endPoint, float(j) / samples);
			result.Add(resampledPoint);
		}
	}
	result.Add(linePoints3D[linePoints3D.Num() - 1]); // Add original last point
	return result;
}

TArray<FDataSourceTypeMapping> UVectorObject::GetLayerTypesUsingTypesIndices() const
{
	check(false);
	return {};
	/*const auto& config = UConfig::GetInstance();
	UVectorLayer* vectorLayer = *config->datamodel->vector_layers.FindByPredicate([this](UVectorLayer* a_vectorLayer)
	{
		return nullptr != a_vectorLayer->data.FindByPredicate([this](const UVectorObject* a_vectorObject)
		{
			return a_vectorObject->GetUniqueID() == this->GetUniqueID();
		});
	});
	TArray<UStringMap*> result;
	for (const int32 typeIndex : types) {
		result.Add(vectorLayer->types[typeIndex]);
	}
	return result;*/
}

TArray<FString> UVectorObject::GetLayerTypesFieldValuesUsingTypesIndices(const FString& a_fieldName) const
{
	check(false);
	return {};
	//TArray<UStringMap*> layerTypes = GetLayerTypesUsingTypesIndices();
	//TArray<FString> result;
	//for (const auto& layerType : layerTypes) {
	//	result.Add(layerType->m_values[a_fieldName]);
	//}
	//return result;
}
