#pragma once

#include "GeoCoordinateSystemTransformation.generated.h"

typedef struct PJconsts PJ;

UCLASS()
class UGeoCoordinateSystemTransformation: public UObject
{
	GENERATED_BODY()
public:
	static void SetupProjContext();
	static void DestroyProjContext();

	static UGeoCoordinateSystemTransformation* TryCreateFromProjWKT(const FString& a_sourceProjection, const FString& a_targetProjection);

	virtual void BeginDestroy() override;

	FVector4 PerformTransformation(FVector4 a_sourceCoordinates) const;

private:
	PJ* m_projTransformation;
};