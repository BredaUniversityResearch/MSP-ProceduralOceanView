#include "PRPDisplayModeLineSpline.h"

#include "Components/SplineComponent.h"
#include "DataPool/DataPool.h"
#include "MSP_ProceduralGen/MSP_ProceduralGen.h"
#include "ProceduralRulePack/ProceduralRulePackActorMaterial.h"

void UPRPDisplayModeLineSpline::EnableDisplayMode_Implementation(UDataPool* a_dataPool)
{
	Super::EnableDisplayMode_Implementation(a_dataPool);

	FName dataSourceName = LineDataSource.Resolve();

	FDataPoolLines dataSource;
	if (a_dataPool->TryGetData(dataSourceName, dataSource))
	{
		CreateActorsFromDataSource(dataSource);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(234546255, 10.0f, FColor::Orange, FString::Printf(TEXT("PRPDisplayModeLineSpline failed to find target layer %ls"), *dataSourceName.ToString()));
	}
}

void UPRPDisplayModeLineSpline::DisableDisplayMode_Implementation(UDataPool* a_dataPool)
{
	Super::DisableDisplayMode_Implementation(a_dataPool);

	DestroyActiveActors();
}

void UPRPDisplayModeLineSpline::CreateActorsFromDataSource(const FDataPoolLines& DataLines)
{
	for(FDataPoolLine lineSpline : DataLines.m_lines)
	{
		if (lineSpline.m_points.m_vectorArray.Num() > 0)
		{
			AActor* actor = GetWorld()->SpawnActor(ActorMaterialToSpawn.Get(), &lineSpline.m_points.m_vectorArray[0]);
			AProceduralRulePackActorMaterial* actorTyped = Cast<AProceduralRulePackActorMaterial>(actor);
			ensure(actorTyped != nullptr);
			
			actorTyped->DisplayMaterial = this->DisplayMaterial;

			if (USplineComponent* spline = actorTyped->GetComponentByClass<USplineComponent>())
			{
				spline->SetSplinePoints(lineSpline.m_points.m_vectorArray, ESplineCoordinateSpace::World);
			}
			else
			{
				UE_LOG(LogMspProceduralGen, Warning, TEXT("Failed to find Spline Component on actor %s. LineSpline Display Method will not work correctly"), *ActorMaterialToSpawn->GetFName().ToString());
			}

			actorTyped->PRPActorSetupComplete();
			ActiveActors.Add(actorTyped);	
		}
		else
		{
			FName dataSourceName = LineDataSource.Resolve();
			UE_LOG(LogMspProceduralGen, Warning, TEXT("Found line with 0 points for LineSpline display mode. Target datasource %s"), *dataSourceName.ToString());
		}
	}
}

void UPRPDisplayModeLineSpline::DestroyActiveActors()
{
	for (AProceduralRulePackActor* actor : ActiveActors)
	{
		GetWorld()->DestroyActor(actor);
	}
	ActiveActors.Empty();
}
