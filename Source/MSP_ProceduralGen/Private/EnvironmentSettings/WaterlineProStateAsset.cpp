#include "WaterlineProStateAsset.h"

FWaterlineProState FWaterlineProState::Lerp(const FWaterlineProState& From, const FWaterlineProState& To, float Alpha)
{
	FWaterlineProState result{};

	result.SimulationScaleFFT = FMath::Lerp(From.SimulationScaleFFT, To.SimulationScaleFFT, Alpha);

	result.WindSpeed = FMath::Lerp(From.WindSpeed, To.WindSpeed, Alpha);
	result.WindContributionFFT = FMath::Lerp(From.WindContributionFFT, To.WindContributionFFT, Alpha);

	result.FFTGravity = FMath::Lerp(From.FFTGravity, To.FFTGravity, Alpha);

	result.WaveAmplitudeFFT = FMath::Lerp(From.WaveAmplitudeFFT, To.WaveAmplitudeFFT, Alpha);
	result.WaveSpeed = FMath::Lerp(From.WaveSpeed, To.WaveSpeed, Alpha);
	result.WaveTile = FMath::Lerp(From.WaveTile, To.WaveTile, Alpha);
	result.WaveHeightFFT = FMath::Lerp(From.WaveHeightFFT, To.WaveHeightFFT, Alpha);
	result.WaveChoppynessFFT = FMath::Lerp(From.WaveChoppynessFFT, To.WaveChoppynessFFT, Alpha);

	return result;
}

FWaterlineProState UWaterlineProStateAsset::GetInterpolatedWaterlineProState(const UWaterlineProStateAsset* From, const UWaterlineProStateAsset* To, float Alpha)
{
	return FWaterlineProState::Lerp(From->State, To->State, Alpha);
}
