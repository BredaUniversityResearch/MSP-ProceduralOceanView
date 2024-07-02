#include "PRPDisplayModePointActor.h"

#include "DataPool/DataPool.h"
#include "ProceduralRulePack/ProceduralRulePackActor.h"

void UPRPDisplayModePointActor::EnableDisplayMode_Implementation(UDataPool* a_dataPool)
{
	Super::EnableDisplayMode_Implementation(a_dataPool);

	FDataPoolPoints dataSource;
	if (a_dataPool->TryGetData(TargetPointsDataSource, dataSource))
	{
		CreateActorsFromDataSource(dataSource);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(234546255, 10.0f, FColor::Orange, FString::Printf(TEXT("PRPDisplayModePointActor failed to find target layer %ls"), *TargetPointsDataSource.ToString()));
	}
}

void UPRPDisplayModePointActor::DisableDisplayMode_Implementation(UDataPool* a_dataPool)
{
	Super::DisableDisplayMode_Implementation(a_dataPool);

	DestroyActiveActors();
}

void UPRPDisplayModePointActor::CreateActorsFromDataSource(const FDataPoolPoints& ActorPoints)
{
	for(FDataPoolPoint windFarmPosition : ActorPoints.m_points)
	{		
		AActor* actor = GetWorld()->SpawnActor(ActorToSpawn.Get(), &windFarmPosition.m_vector);
		AProceduralRulePackActor* actorTyped = Cast<AProceduralRulePackActor>(actor);
		ensure(actorTyped != nullptr);

		// Initialize Actor with the Metadata 
		actorTyped->ActorMetaData.Empty();
		for (TPair<FString, FString> metadataPair : windFarmPosition.m_metadata.Array()) 
		{
			actorTyped->ActorMetaData.Add(metadataPair.Key, metadataPair.Value);
		}		
		actorTyped->PRPActorSetupComplete();

		ActiveActors.Add(actorTyped);
	}
}

void UPRPDisplayModePointActor::DestroyActiveActors()
{
	for (AProceduralRulePackActor* actor : ActiveActors)
	{
		//auto fadeController = actor->FindComponentByClass<UMSPObjectFadeControllerComponent>();

		actor->OnStartDespawn();
	}
	ActiveActors.Empty();
}
 