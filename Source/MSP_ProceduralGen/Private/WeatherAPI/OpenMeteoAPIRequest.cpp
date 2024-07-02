#include "OpenMeteoAPIRequest.h"

#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

bool FOpenMeteoAPI::RequestWeatherInfo(FLatLonCoordinate Location, FOpenMeteoWeatherInfoReceived OnCompleted)
{
	FString requestURL = FString::Printf(TEXT("https://api.open-meteo.com/v1/forecast?latitude=%f&longitude=%f&current=temperature_2m,precipitation,cloud_cover,wind_speed_10m,wind_direction_10m"), Location.Latitude, Location.Longitude);

	auto request = FHttpModule::Get().CreateRequest();
	request->SetVerb(TEXT("GET"));
	request->SetURL(requestURL);

	request->OnProcessRequestComplete().BindLambda([OnCompleted](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully) {

		bool success = false;
		FOpenMeteoWeatherResult result {};
		if (bConnectedSuccessfully)
		{
			FString textResponse = Response->GetContentAsString();
			if (Response->GetResponseCode() == 200)
			{
				TSharedPtr<FJsonObject> jsonObject;
				auto reader = TJsonReaderFactory<>::Create(textResponse);
				if (FJsonSerializer::Deserialize(reader, jsonObject))
				{
					result = ParseWeatherResultFromJson(*jsonObject.Get());
					success = true;
				}
			}
		}
		OnCompleted.ExecuteIfBound(success, result);
	});

	return request->ProcessRequest();
}

bool FOpenMeteoAPI::RequestMarineInfo(FLatLonCoordinate Location, FOpenMeteoMarineInfoReceived OnCompleted)
{
	FString requestURL = FString::Printf(TEXT("https://marine-api.open-meteo.com/v1/marine?latitude=%f&longitude=%f&current=wave_height,wave_direction,wave_period"), Location.Latitude, Location.Longitude);

	auto request = FHttpModule::Get().CreateRequest();
	request->SetVerb(TEXT("GET"));
	request->SetURL(requestURL);

	request->OnProcessRequestComplete().BindLambda([OnCompleted](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully) {

		bool success = false;
		FOpenMeteoMarineResult result {};
		if (bConnectedSuccessfully)
		{
			FString textResponse = Response->GetContentAsString();
			if (Response->GetResponseCode() == 200)
			{
				TSharedPtr<FJsonObject> jsonObject;
				auto reader = TJsonReaderFactory<>::Create(textResponse);
				if (FJsonSerializer::Deserialize(reader, jsonObject))
				{
					result = ParseMarineResultFromJson(*jsonObject.Get());
					success = true;
				}
			}
		}
		OnCompleted.ExecuteIfBound(success, result);
	});

	return request->ProcessRequest();
}

FOpenMeteoWeatherResult FOpenMeteoAPI::ParseWeatherResultFromJson(const FJsonObject& DocumentRoot)
{
	FOpenMeteoWeatherResult result = {};

	const TSharedPtr<FJsonObject>* currentObject;
	if (DocumentRoot.TryGetObjectField(TEXT("current"), currentObject))
	{
		(*currentObject)->TryGetNumberField(TEXT("temperature_2m"), result.Temperature_2m);
		(*currentObject)->TryGetNumberField(TEXT("precipitation"), result.Precipitation);
		(*currentObject)->TryGetNumberField(TEXT("cloud_cover"), result.CloudCover);
		(*currentObject)->TryGetNumberField(TEXT("wind_speed_10m"), result.WindSpeed_10m);
		(*currentObject)->TryGetNumberField(TEXT("wind_direction_10m"), result.WindDirection_10m);
	}

	return result;
}

FOpenMeteoMarineResult FOpenMeteoAPI::ParseMarineResultFromJson(const FJsonObject& DocumentRoot)
{
	FOpenMeteoMarineResult result = {};

	const TSharedPtr<FJsonObject>* currentObject;
	if (DocumentRoot.TryGetObjectField(TEXT("current"), currentObject))
	{
		(*currentObject)->TryGetNumberField(TEXT("wave_height"), result.WaveHeight);
		(*currentObject)->TryGetNumberField(TEXT("wave_direction"), result.WaveDirection);
		(*currentObject)->TryGetNumberField(TEXT("wave_period"), result.WavePeriod);
	}

	return result;
}
