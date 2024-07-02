#include "EnvironmentStateSettings.h"

void UEnvironmentStateSettings::SetTimeOfDay(float TimeOfDay)
{
	EnvironmentState.TimeOfDay = TimeOfDay;
}

void UEnvironmentStateSettings::SetPrecipitation(float Precipitation)
{
	EnvironmentState.Precipitation = Precipitation;
}

void UEnvironmentStateSettings::SetCloudCover(float CloudCover)
{
	EnvironmentState.CloudCover = CloudCover;
}

void UEnvironmentStateSettings::SetVisibility(float Visibility)
{
	EnvironmentState.Visibility = Visibility;
}

void UEnvironmentStateSettings::SetWindSpeed(float WindSpeed)
{
	EnvironmentState.WindSpeed = WindSpeed;
}

void UEnvironmentStateSettings::SetCurrentSpeed(float CurrentSpeed)
{
	EnvironmentState.CurrentSpeed = CurrentSpeed;
}

void UEnvironmentStateSettings::SetWaveHeight(float WaveHeight)
{
	EnvironmentState.WaveHeight = WaveHeight;
}

float UEnvironmentStateSettings::GetTimeOfDay() const
{
	return EnvironmentState.TimeOfDay;
}

float UEnvironmentStateSettings::GetPrecipitation() const
{
	return EnvironmentState.Precipitation;
}

float UEnvironmentStateSettings::GetCloudCover() const
{
	return EnvironmentState.CloudCover;
}

float UEnvironmentStateSettings::GetVisibility() const
{
	return EnvironmentState.Visibility;
}

float UEnvironmentStateSettings::GetWindSpeed() const
{
	return EnvironmentState.WindSpeed;
}

float UEnvironmentStateSettings::GetCurrentSpeed() const
{
	return EnvironmentState.CurrentSpeed;
}

float UEnvironmentStateSettings::GetWaveHeight() const
{
    return EnvironmentState.WaveHeight;
}

void UEnvironmentStateSettings::SetWindDirection(float WindDirection)
{
	EnvironmentState.WindDirection = WindDirection;
}

void UEnvironmentStateSettings::SetCurrentDirection(float CurrentDirection)
{
	EnvironmentState.CurrentDirection = CurrentDirection;
}

float UEnvironmentStateSettings::GetWindDirection() const
{
	return EnvironmentState.WindDirection;
}

float UEnvironmentStateSettings::GetCurrentDirection() const
{
	return EnvironmentState.CurrentDirection;
}
