#pragma once
#include "ProceduralRulePack/ProceduralRulePackDisplayMode.h"

#include "PRPDisplayModePointActor.generated.h"

struct FDataPoolPoints;
class UDataPool;

UCLASS(Blueprintable, BlueprintType)
class UPRPDisplayModePointActor: public UProceduralRulePackDisplayMode
{
	GENERATED_BODY()
public:
	virtual void EnableDisplayMode_Implementation(UDataPool* a_dataPool) override;
	virtual void DisableDisplayMode_Implementation(UDataPool* a_dataPool) override;

protected:

	UPROPERTY()
	TArray<AProceduralRulePackActor*> ActiveActors;

private:
	void CreateActorsFromDataSource(const FDataPoolPoints& ActorPoints);
	void DestroyActiveActors();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProceduralRulePackActor> ActorToSpawn = {};

	UPROPERTY(EditDefaultsOnly)
	FName TargetPointsDataSource = {};
};
