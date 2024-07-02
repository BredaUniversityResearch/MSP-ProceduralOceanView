#pragma once

#include "DataSourceTypeMapping.h"
#include "UObject/Object.h"
#include "Common/FloatArrayWrapper.h"
#include "Common/FloatArrayContainerWrapper.h"
#include "Common/StringMap.h"
#include "VectorObject.generated.h"

UCLASS(BlueprintType)
// ReSharper disable CppInconsistentNaming, need to map with the json
class UVectorObject : public UObject
{
	GENERATED_BODY()

public:
	static constexpr float DEFAULT_SEGMENT_LENGTH = 100000.0f;

	UPROPERTY(SkipSerialization, BlueprintReadOnly, Category = "Config|Data model|Layer|Vector layer|Data", DisplayName="Array of vertex positions")
		TArray<UFloatArrayWrapper*> points;
	UPROPERTY(SkipSerialization, BlueprintReadOnly, Category = "Config|Data model|Layer|Vector layer|Data", DisplayName="Array of gaps, each of which is an array of vertex positions")
		 TArray<UFloatArrayContainerWrapper*> gaps;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Vector layer|Data", DisplayName="A list of indices, of the layer types array")
		TArray<int32> types;
	UPROPERTY(SkipSerialization, BlueprintReadOnly, Category = "Config|Data model|Layer|Vector layer|Data", DisplayName="Object containing any metadata for the geometry")
		UStringMap* metadata;

	UFUNCTION(BlueprintCallable, Category="Config|Data model|Layer|Vector layer|Data", DisplayName="Get points as world Vector2D")
		TArray<FVector2D> GetPointsAsWorldVector2D() const;
	UFUNCTION(BlueprintCallable, Category="Config|Data model|Layer|Vector layer|Data", DisplayName="Get points as world Vector")
		TArray<FVector> GetPointsAsWorldVector() const;
	UFUNCTION(BlueprintCallable, Category="Config|Data model|Layer|Vector layer|Data", DisplayName="Get metadata as StringMap")
		const TMap<FString, FString>& GetMetadataAsStringMap() const
		{
			return metadata->m_values;
		}
	UFUNCTION(BlueprintCallable, Category="Config|Data model|Layer|Vector layer|Data", DisplayName="Get metadata value")
		const FString& GetMetadataValue(const FString& a_key) const;
	UFUNCTION(BlueprintCallable, Category="Config|Data model|Layer|Vector layer|Data", DisplayName="Get metadata value as float")
		float GetMetadataValueAsFloat(const FString& a_key) const;
	UFUNCTION(BlueprintCallable, Category="Config|Data model|Layer|Vector layer|Data", DisplayName="Get metadata value as int32")
		int32 GetMetadataValueAsInt(const FString& a_key) const;
	UFUNCTION(BlueprintCallable, Category="Config|Data model|Layer|Vector layer|Data", DisplayName="Get metadata value as bool")
		bool GetMetadataValueAsBool(const FString& a_key) const;
	UFUNCTION(BlueprintCallable, Category="Config|Data model|Layer|Vector layer|Data", DisplayName="Resample line points by given segment length")
		TArray<FVector> ResampleLinePoints(float a_segmentLength) const;
	UFUNCTION(BlueprintCallable, Category="Config|Data model|Layer|Vector layer|Data", DisplayName="Get layer \"Types\" using \"Types\" indices")
		TArray<FDataSourceTypeMapping> GetLayerTypesUsingTypesIndices() const;
	UFUNCTION(BlueprintCallable, Category="Config|Data model|Layer|Vector layer|Data", DisplayName="Get a layer \"Types\" field values using \"Types\" indices")
		TArray<FString> GetLayerTypesFieldValuesUsingTypesIndices(const FString& a_fieldName) const;
};
