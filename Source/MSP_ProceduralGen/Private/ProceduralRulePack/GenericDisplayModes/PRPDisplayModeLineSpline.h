#pragma once
#include "ProceduralRulePack/ProceduralRulePackDisplayMode.h"
#include <ProceduralRulePack/ProceduralRulePackActorMaterial.h>
#include "DataPool\DataPoolDataSourceRef.h"
#include "Materials\MaterialInstance.h"
#include "PRPDisplayModeLineSpline.generated.h"

struct FDataPoolLines;
class UDataPool;

UCLASS(Blueprintable, BlueprintType)
class UPRPDisplayModeLineSpline: public UProceduralRulePackDisplayMode
{
	GENERATED_BODY()
public:
	virtual void EnableDisplayMode_Implementation(UDataPool* a_dataPool) override;
	virtual void DisableDisplayMode_Implementation(UDataPool* a_dataPool) override;

private:
	void CreateActorsFromDataSource(const FDataPoolLines& DataLines);
	void DestroyActiveActors();

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AProceduralRulePackActorMaterial> ActorMaterialToSpawn = {};

	UPROPERTY(EditDefaultsOnly)
		FDataPoolDataSourceRef LineDataSource;

	UPROPERTY(EditDefaultsOnly)
		UMaterialInterface* DisplayMaterial = {};
		
	UPROPERTY()
	TArray<AProceduralRulePackActor*> ActiveActors;
};