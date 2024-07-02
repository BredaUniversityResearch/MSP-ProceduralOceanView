#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkyCreatorPresetCollection.generated.h"

struct FEnvironmentState;
USTRUCT()
struct FSkyCreatorPresetDefinition
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	class USkyCreatorWeatherPreset* WeatherSettings{ nullptr };
	UPROPERTY(EditAnywhere, Meta = (UIMin = 0.0, UIMax = 1.0))
	float CloudCover{};
	//UPROPERTY(EditAnywhere, Meta = (UIMin = 0.0))
	//float CloudCoverWeight = 1.0f;
	
	float CalculateMatchMetricFor(const FEnvironmentState& State) const;
	static float GetUnweightedDifference(float a_current, float a_reference);
};

UCLASS()
class USkyCreatorPresetCollection : public UDataAsset
{
	GENERATED_BODY()
public:
	const USkyCreatorWeatherPreset* FindBestMatchPresetForConditions(const FEnvironmentState& State) const;

private:
	UPROPERTY(EditDefaultsOnly)
	TArray<FSkyCreatorPresetDefinition> AvailablePresets;
};
