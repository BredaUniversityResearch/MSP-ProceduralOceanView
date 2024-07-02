//#pragma once
//
//#include "CoreMinimal.h"
//#include "Engine/DataAsset.h"
//#include "WaterBodyPresetCollection.generated.h"
//
//struct FEnvironmentState;
//USTRUCT()
//struct FWaterBodyPresetDefinition
//{
//	GENERATED_BODY()
//public:
//	UPROPERTY(EditAnywhere)
//	class UWaterWavesAsset* WaveGenerationSettings{ nullptr };
//	UPROPERTY(EditAnywhere, Meta = (UIMin = 0.0, UIMax = 1.0))
//	float WindSpeed{};
//	UPROPERTY(EditAnywhere, Meta = (UIMin = 0.0))
//	float WindSpeedWeight = 1.0f;
//
//	float CalculateMatchMetricFor(const FEnvironmentState& State) const;
//
//	static float GetUnweightedDifference(float a_current, float a_reference);
//};
//
//UCLASS()
//class UWaterBodyPresetCollection : public UDataAsset
//{
//	GENERATED_BODY()
//public:
//	const class UWaterWavesAsset* FindBestMatchPresetForConditions(const FEnvironmentState& State) const;
//
//private:
//	UPROPERTY(EditDefaultsOnly)
//	TArray<FWaterBodyPresetDefinition> AvailablePresets;
//};
