#include "GeoCoordinateHelper.h"

#include "GeoCoordinateSystemTransformation.h"
#include "MSPProcGenGameMode.h"
#include "Config/Config.h"

FLatLonCoordinate UGeoCoordinateHelper::TransformWorldPositionToLatLon(const UObject* WorldContext, FVector WorldPosition)
{
	auto* gameMode = WorldContext->GetWorld()->GetAuthGameMode<AMSPProcGenGameMode>();
	verify(gameMode != nullptr);
	FVector sourceCoordinate = WorldPositionToSourceCoordinateSystem(WorldPosition, gameMode->SimulationProjection.WorldOffset);

	FVector4 transformed = gameMode->LoadedConfigToLatLon->PerformTransformation(sourceCoordinate);

	FLatLonCoordinate coord = {
		.Latitude = transformed.Y,
		.Longitude = transformed.X
	};
	return coord;
}

FString UGeoCoordinateHelper::GetFormattedLatLonCoordinate(const FLatLonCoordinate &Coordinate)
{
	auto latDeg = FMath::FloorToInt(Coordinate.Latitude);
	auto latMin = FMath::FloorToInt((Coordinate.Latitude - latDeg) * 60.0);
	auto latSec = FMath::FloorToInt(((Coordinate.Latitude - latDeg) * 60.0 - latMin) * 60.0);

	auto lonDeg = FMath::FloorToInt(Coordinate.Longitude);
	auto lonMin = FMath::FloorToInt((Coordinate.Longitude - lonDeg) * 60.0);
	auto lonSec = FMath::FloorToInt(((Coordinate.Longitude - lonDeg) * 60.0 - lonMin) * 60.0);

	return FString::Printf(TEXT("Lat: %d°%d'%d\"%s Lon: %d°%d'%d\"%s"),
		latDeg, latMin, latSec, Coordinate.Latitude >= 0.0 ? TEXT("N") : TEXT("S"),
		lonDeg, lonMin, lonSec, Coordinate.Longitude >= 0.0 ? TEXT("E") : TEXT("W"));
}

FVector UGeoCoordinateHelper::WorldPositionToSourceCoordinateSystem(FVector WorldPosition, FVector2D WorldOffset)
{
	WorldPosition += FVector(WorldOffset, 0.0);
	WorldPosition *= FVector(1.0 / UConfig::MetersToUnrealUnits);
	return WorldPosition;
}
