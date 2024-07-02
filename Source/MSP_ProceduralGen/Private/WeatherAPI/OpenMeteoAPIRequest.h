#pragma once
#include "GeoProjection/GeoCoordinateHelper.h"

#include "OpenMeteoAPIRequest.generated.h"

USTRUCT()
struct FOpenMeteoWeatherResult
{
	GENERATED_BODY()

	float Temperature_2m; //Celsius
	float Precipitation; //mm
	float CloudCover; //% 0..100
	float WindSpeed_10m; //km/h
	float WindDirection_10m; //Degrees 0..360
};

USTRUCT()
struct FOpenMeteoMarineResult
{
	GENERATED_BODY()

	float WaveHeight; //meters
	float WaveDirection; //Degrees 0..360
	float WavePeriod; //seconds
};

DECLARE_DELEGATE_TwoParams(FOpenMeteoWeatherInfoReceived, bool /*Success*/, const FOpenMeteoWeatherResult& /*Values*/);
DECLARE_DELEGATE_TwoParams(FOpenMeteoMarineInfoReceived, bool /*Success*/, const FOpenMeteoMarineResult& /*Values*/);

class FOpenMeteoAPI
{
public:
	static bool RequestWeatherInfo(FLatLonCoordinate Location, FOpenMeteoWeatherInfoReceived OnCompleted);
	static bool RequestMarineInfo(FLatLonCoordinate Location, FOpenMeteoMarineInfoReceived OnCompleted);

private:
	static FOpenMeteoWeatherResult ParseWeatherResultFromJson(const FJsonObject& DocumentRoot);
	static FOpenMeteoMarineResult ParseMarineResultFromJson(const FJsonObject& DocumentRoot);
};
