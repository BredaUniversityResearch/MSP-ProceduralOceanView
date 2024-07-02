#pragma once

#include "CoreMinimal.h"
#include "EnvironmentState.h"
#include "EnvironmentStateSettings.generated.h"


UCLASS(BlueprintType)
class UEnvironmentStateSettings : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable) //[0..1]
	void SetTimeOfDay(float TimeOfDay);
	UFUNCTION(BlueprintCallable) //[0..1]
	void SetPrecipitation(float Precipitation);
	UFUNCTION(BlueprintCallable) //[0..1]
	void SetCloudCover(float CloudCover);
	UFUNCTION(BlueprintCallable) //[0..1]
	void SetVisibility(float Visibility);
	UFUNCTION(BlueprintCallable) //[0..1]
	void SetWindSpeed(float WindSpeed);
	UFUNCTION(BlueprintCallable) //[0..1]
	void SetCurrentSpeed(float CurrentSpeed);
	UFUNCTION(BlueprintCallable) //[0..1]
	void SetWaveHeight(float WaveHeight);

	UFUNCTION(BlueprintCallable)
	float GetTimeOfDay() const;
	UFUNCTION(BlueprintCallable) //[0..1]
	float GetPrecipitation() const;
	UFUNCTION(BlueprintCallable) //[0..1]
	float GetCloudCover() const;
	UFUNCTION(BlueprintCallable) //[0..1]
	float GetVisibility() const;
	UFUNCTION(BlueprintCallable) //[0..1]
	float GetWindSpeed() const;
	UFUNCTION(BlueprintCallable) //[0..1]
	float GetCurrentSpeed() const;
	UFUNCTION(BlueprintCallable) //[0..1]
	float GetWaveHeight() const;

	UFUNCTION(BlueprintCallable) //[0..360]
	void SetWindDirection(float WindDirection);
	UFUNCTION(BlueprintCallable) //[0..360]
	void SetCurrentDirection(float CurrentDirection);

	UFUNCTION(BlueprintCallable) //[0..360]
	float GetWindDirection() const;
	UFUNCTION(BlueprintCallable) //[0..360]
	float GetCurrentDirection() const;


	UPROPERTY(EditAnywhere, Meta = (ShowOnlyInnerProperties))
	FEnvironmentState EnvironmentState;
};
