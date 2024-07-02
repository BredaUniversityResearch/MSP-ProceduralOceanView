#pragma once

#include "FloatArrayWrapper.generated.h"

UCLASS(BlueprintType)
class UFloatArrayWrapper : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Config|Common", DisplayName="Raw values")
		TArray<float> m_values;

	FVector2D GetRawValuesAsVector2D() const;
	FVector2D AsProjectedVector2D(const FTransform& a_transform) const;
	FVector AsProjectedVector(const FTransform& a_transform) const;
	UFUNCTION(BlueprintCallable, Category = "Config|Common", DisplayName="As world Vector2D")
		FVector2D AsWorldVector2D() const;
	UFUNCTION(BlueprintCallable, Category = "Config|Common", DisplayName="As world Vector")
		FVector AsWorldVector() const;
};
