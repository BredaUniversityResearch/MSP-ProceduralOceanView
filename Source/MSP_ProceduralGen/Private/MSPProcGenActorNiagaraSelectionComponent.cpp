#include "MSPProcGenActorNiagaraSelectionComponent.h"

#include "Components/BoxComponent.h"
#include "Intersection/IntersectionUtil.h"
#include "MSP_ProceduralGen/MSP_ProceduralGen.h"

UMSPProcGenActorNiagaraSelectionComponent::UMSPProcGenActorNiagaraSelectionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{	
}

void UMSPProcGenActorNiagaraSelectionComponent::BeginPlay()
{
	Super::BeginPlay();

	TargetBoxComponent = Cast<UBoxComponent>(BoxComponentReference.GetComponent(GetOwner()));
	if (TargetBoxComponent == nullptr)
	{
		UE_LOG(LogMspProceduralGen, Warning, TEXT("MSPProcGenActorNiagaraSelectionComponent on actor %s does not have a valid reference to a box component"), *GetOwner()->GetName());
		verify(TargetBoxComponent != nullptr);
	}
}

void UMSPProcGenActorNiagaraSelectionComponent::SetNiagaraParticleData(TArray<FMSPProcGenNiagaraSelectionParticleData> Points)
{
	LatestParticleData = MoveTemp(Points);

	if (TargetBoxComponent != nullptr)
	{
		FVector Min = FVector(TNumericLimits<double>::Max());
		FVector Max = FVector(TNumericLimits<double>::Min());

		for(const FMSPProcGenNiagaraSelectionParticleData& particleData : LatestParticleData)
		{
			Min = Min.ComponentMin(particleData.Location);
			Max = Max.ComponentMax(particleData.Location);
		}

		FVector boxExtents = Max - Min;
		FVector boxLocation = Min + (boxExtents / 2);

		TargetBoxComponent->SetBoxExtent(boxExtents);
		TargetBoxComponent->SetWorldLocation(boxLocation);
	}
}

bool UMSPProcGenActorNiagaraSelectionComponent::PerformSubSelection(const FCollisionShape& TraceShape, const FVector& LineTraceStart, const FVector& LineTraceEnd)
{
	FVector LineTraceDelta = LineTraceEnd - LineTraceStart;
	double LineTraceLength = LineTraceDelta.Length();
	FVector LineTraceDirection = LineTraceDelta * (1.0 / LineTraceLength);

	double meshRadius = LatestParticleData[0].MeshBounds.Length() / 2.0f;
	for (const FMSPProcGenNiagaraSelectionParticleData& particle : LatestParticleData)
	{
		UE::Geometry::FLinearIntersection intersectResult;
		if (IntersectionUtil::RaySphereIntersection(LineTraceStart, LineTraceDirection, particle.Location, meshRadius, intersectResult))
		{
			return true;
		}
	}
	return false;
}
