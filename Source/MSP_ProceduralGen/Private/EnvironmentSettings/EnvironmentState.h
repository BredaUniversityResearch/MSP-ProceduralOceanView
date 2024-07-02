#pragma once

#include "EnvironmentState.generated.h"

USTRUCT(BlueprintType)
struct FEnvironmentState
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (UIMin = 0.0, UIMax = 1.0))
		float TimeOfDay = 0.45f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (UIMin = 0.0, UIMax = 1.0))
		float Precipitation = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (UIMin = 0.0, UIMax = 1.0))
		float CloudCover = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (UIMin = 0.0, UIMax = 1.0))
		float Visibility = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (UIMin = 0.0, UIMax = 1.0))
		float WindSpeed = 15.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (UIMin = 0.0, UIMax = 1.0))
		float CurrentSpeed = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (UIMin = 0.0, UIMax = 1.0))
		float WaveHeight = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (UIMin = 0.0, UIMax = 360.0))
		float WindDirection = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (UIMin = 0.0, UIMax = 360.0))
		float CurrentDirection = 0.0f;
};
