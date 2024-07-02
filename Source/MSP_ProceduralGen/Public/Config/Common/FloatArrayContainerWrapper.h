#pragma once

#include "FloatArrayWrapper.h"
#include "FloatArrayContainerWrapper.generated.h"

UCLASS(BlueprintType)
class UFloatArrayContainerWrapper : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Config|Common", DisplayName="Raw values")
		TArray<UFloatArrayWrapper*> m_values;

	TArray<FVector2D> AsProjectedVector2DArray(const FTransform& a_transform) const;
	TArray<FVector> AsProjectedVectorArray(const FTransform& a_transform) const;
	static TArray<FVector> AsProjectedVectorArray(
		const TArray<UFloatArrayWrapper*>& a_floatArrayContainer,
		const FTransform& a_transform
	);

	UFUNCTION(BlueprintCallable, Category = "Config|Common", DisplayName="As raw values Vector2D array")
		TArray<FVector2D> GetRawValuesAsVector2DArray() const;
	UFUNCTION(BlueprintCallable, Category = "Config|Common", DisplayName="As world Vector2D array")
		static TArray<FVector2D> AsWorldVector2DArray(const TArray<UFloatArrayWrapper*>& a_floatArrayContainer);
	UFUNCTION(BlueprintCallable, Category = "Config|Common", DisplayName="As world Vector array")
		static TArray<FVector> AsWorldVectorArray(const TArray<UFloatArrayWrapper*>& a_floatArrayContainer);
	UFUNCTION(BlueprintCallable, Category = "Config|Common", DisplayName="Get values as world Vector2D array")
		TArray<FVector2D> GetValuesAsWorldVector2DArray() const;
	UFUNCTION(BlueprintCallable, Category = "Config|Common", DisplayName="Get values as world Vector array")
		TArray<FVector> GetValuesAsWorldVectorArray() const;
};
