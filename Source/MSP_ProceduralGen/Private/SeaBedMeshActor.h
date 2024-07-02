#pragma once
#include "ProceduralMeshComponent.h"
#include "DataPool/DataPool.h"

#include "SeaBedMeshActor.generated.h"

UCLASS()
class ASeaBedMeshActor: public AActor
{
	GENERATED_BODY()
public:
	ASeaBedMeshActor(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProceduralMeshComponent* MeshComponent;

	bool GenerateProceduralMeshData(FIntVector2 VertexResolution, int32 MeshSmoothingAmount, FBox2D TargetWorldArea, UProceduralMeshComponent* Target);

private:
	UFUNCTION()
	void OnBathymetryDataChanged(const FName& UpdatedDataSourceName);

	UPROPERTY(EditAnywhere)
	int32 VertexResolutionXY = 256;
	UPROPERTY(EditAnywhere)
	int32 MeshSmoothing = 5;

	FOnDataSourceDataUpdatedMulticastDelegate::FDelegate m_bathymetryDataUpdatedDelegate;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* MeshMaterial{nullptr};
};