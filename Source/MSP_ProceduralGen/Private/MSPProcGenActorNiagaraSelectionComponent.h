#pragma once 

#include "MSPProcGenActorNiagaraSelectionComponent.generated.h"

class UBoxComponent;

USTRUCT(BlueprintType)
struct FMSPProcGenNiagaraSelectionParticleData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FVector Location;
	UPROPERTY(BlueprintReadWrite)
	float Size;
	UPROPERTY(BlueprintReadWrite)
	FVector MeshBounds;
};

UCLASS(meta=(BlueprintSpawnableComponent))
class UMSPProcGenActorNiagaraSelectionComponent: public UActorComponent
{
	GENERATED_BODY()
public:
	UMSPProcGenActorNiagaraSelectionComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetNiagaraParticleData(TArray<FMSPProcGenNiagaraSelectionParticleData> Points);

	bool PerformSubSelection(const FCollisionShape& TraceShape, const FVector& LineTraceStart, const FVector& LineTraceEnd);

private:
	UPROPERTY(EditAnywhere)
	FSoftComponentReference BoxComponentReference;
	UPROPERTY(Transient)
	UBoxComponent* TargetBoxComponent;
	UPROPERTY(Transient)
	TArray<FMSPProcGenNiagaraSelectionParticleData> LatestParticleData;
};