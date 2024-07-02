//#include "WaterBodyPresetCollection.h"
//
//#include "EnvironmentStateSettings.h"
//
//float FWaterBodyPresetDefinition::CalculateMatchMetricFor(const FEnvironmentState& State) const
//{
//	float totalWeight = WindSpeedWeight;
//
//	float unNormalized = (GetUnweightedDifference(State.WindSpeed, WindSpeed) * WindSpeedWeight)	;
//	return unNormalized / totalWeight;
//}
//
//float FWaterBodyPresetDefinition::GetUnweightedDifference(float a_current, float a_reference)
//{
//	return 1.0f - FMath::Pow(FMath::Abs(a_current - a_reference), 0.5f);
//}
//
//const UWaterWavesAsset* UWaterBodyPresetCollection::FindBestMatchPresetForConditions(const FEnvironmentState& State) const
//{
//	float bestMatchPercentage = 0.0f;
//	const UWaterWavesAsset* bestMatchPreset = nullptr;
//	for (const auto& it : AvailablePresets)
//	{
//		float matchPercentage = it.CalculateMatchMetricFor(State);
//		if (matchPercentage > bestMatchPercentage)
//		{
//			bestMatchPreset = it.WaveGenerationSettings;
//			bestMatchPercentage = matchPercentage;
//		}
//	}
//
//	return bestMatchPreset;
//}
