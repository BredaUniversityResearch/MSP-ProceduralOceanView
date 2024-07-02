#include "GeoCoordinateSystemTransformation.h"

#include "proj.h"
#include "MSP_ProceduralGen/MSP_ProceduralGen.h"

PJ_CONTEXT* g_projContext = nullptr;
uint32 g_projContextCreatedThreadId = 0;

void UGeoCoordinateSystemTransformation::SetupProjContext()
{
	g_projContext = proj_context_create();
	g_projContextCreatedThreadId = FPlatformTLS::GetCurrentThreadId();
}

void UGeoCoordinateSystemTransformation::DestroyProjContext()
{
	proj_context_destroy(g_projContext);
	g_projContext = nullptr;
	g_projContextCreatedThreadId = 0;
}

UGeoCoordinateSystemTransformation* UGeoCoordinateSystemTransformation::TryCreateFromProjWKT(const FString& a_sourceProjection, const FString& a_targetProjection)
{
	checkf(g_projContextCreatedThreadId == FPlatformTLS::GetCurrentThreadId(), TEXT("PROJ threading context was created on a different thread than its being used on. We should rethink this."));

	PJ* projection = proj_create_crs_to_crs(g_projContext, StringCast<char>(*a_sourceProjection).Get(), StringCast<ANSICHAR>(*a_targetProjection).Get(), nullptr);

	if (projection == nullptr)
	{
		const char* projError = proj_context_errno_string(g_projContext, proj_context_errno(g_projContext));
		UE_LOG(LogMspProceduralGen, Error, TEXT("PROJ failed to create a transformation between coordinate systems \"%s\" and \"%s\". PROJ error no: %s"), *a_sourceProjection, *a_targetProjection, StringCast<TCHAR>(projError).Get());
	}
	ensure(projection != nullptr);

	if (projection != nullptr)
	{
		UGeoCoordinateSystemTransformation* result = NewObject<UGeoCoordinateSystemTransformation>();
		result->m_projTransformation = projection;
		return result;
	}

	return nullptr;
}

void UGeoCoordinateSystemTransformation::BeginDestroy()
{
	UObject::BeginDestroy();

	proj_destroy(m_projTransformation);
	m_projTransformation = nullptr;
}

FVector4 UGeoCoordinateSystemTransformation::PerformTransformation(FVector4 a_sourceCoordinates) const
{
	PJ_COORD source;
	source.v[0] = a_sourceCoordinates.X;
	source.v[1] = a_sourceCoordinates.Y;
	source.v[2] = a_sourceCoordinates.Z;
	source.v[3] = a_sourceCoordinates.W;
	PJ_COORD resultCoords = proj_trans(m_projTransformation, PJ_FWD, source);

	FVector4 result = FVector4(resultCoords.v[0], resultCoords.v[1], resultCoords.v[2], resultCoords.v[3]);
	return result;
}
