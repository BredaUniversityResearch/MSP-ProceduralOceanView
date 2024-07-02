#pragma once

#include "Engine/DataAsset.h"
#include "WaterlineProStateAsset.generated.h"


USTRUCT(BlueprintType)
struct FWaterlineProState
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SimulationScaleFFT{1000.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WindSpeed{20.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WindContributionFFT{3.0f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FFTGravity{9.81f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaveAmplitudeFFT{0.015f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaveSpeed{4.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaveTile{3500.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaveHeightFFT{5.0f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaveChoppynessFFT{3.0f};

	static FWaterlineProState Lerp(const FWaterlineProState& From, const FWaterlineProState& To, float Alpha);
};

UCLASS(BlueprintType)
class UWaterlineProStateAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta=(ShowOnlyInnerProperties))
	FWaterlineProState State;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FWaterlineProState GetInterpolatedWaterlineProState(const UWaterlineProStateAsset* From, const UWaterlineProStateAsset* To, float Alpha);
};
