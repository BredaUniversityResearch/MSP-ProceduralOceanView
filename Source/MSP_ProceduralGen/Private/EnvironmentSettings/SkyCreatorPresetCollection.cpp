#include "SkyCreatorPresetCollection.h"

#include "EnvironmentStateSettings.h"

float FSkyCreatorPresetDefinition::CalculateMatchMetricFor(const FEnvironmentState& State) const
{
	float totalWeight = 1.0f;

	float unNormalized = (GetUnweightedDifference(State.CloudCover, CloudCover));
	return unNormalized / totalWeight;
}

float FSkyCreatorPresetDefinition::GetUnweightedDifference(float a_current, float a_reference)
{
	return 1.0f - FMath::Pow(FMath::Abs(a_current - a_reference), 0.5f);
}

const USkyCreatorWeatherPreset* USkyCreatorPresetCollection::FindBestMatchPresetForConditions(const FEnvironmentState& State) const
{
	float bestMatchPercentage = 0.0f;
	const USkyCreatorWeatherPreset* bestMatchPreset = nullptr;
	for (const auto& it : AvailablePresets)
	{
		float matchPercentage = it.CalculateMatchMetricFor(State);
		if (matchPercentage > bestMatchPercentage)
		{
			bestMatchPreset = it.WeatherSettings;
			bestMatchPercentage = matchPercentage;
		}
	}

	return bestMatchPreset;
}
 