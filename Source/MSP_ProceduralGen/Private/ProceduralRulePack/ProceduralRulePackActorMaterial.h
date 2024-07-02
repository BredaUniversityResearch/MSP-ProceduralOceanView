// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralRulePack/ProceduralRulePackActor.h"
#include "ProceduralRulePackActorMaterial.generated.h"

/**
 * 
 */
UCLASS()
class AProceduralRulePackActorMaterial : public AProceduralRulePackActor
{
	GENERATED_BODY()
	


public:
	UPROPERTY(BlueprintReadOnly)
		UMaterialInterface* DisplayMaterial = {};

};
