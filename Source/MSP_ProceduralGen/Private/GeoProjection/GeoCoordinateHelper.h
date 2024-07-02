#pragma once 

#include "GeoCoordinateHelper.generated.h"

USTRUCT(BlueprintType)
struct FLatLonCoordinate
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	double Latitude;
	UPROPERTY(BlueprintReadWrite)
	double Longitude;
};

UCLASS()
class UGeoCoordinateHelper: public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContext"))
	static FLatLonCoordinate TransformWorldPositionToLatLon(const UObject* WorldContext, FVector WorldPosition);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FString GetFormattedLatLonCoordinate(const FLatLonCoordinate& Coordinate);

private:
	static FVector WorldPositionToSourceCoordinateSystem(FVector WorldPosition, FVector2D WorldOffset);
};