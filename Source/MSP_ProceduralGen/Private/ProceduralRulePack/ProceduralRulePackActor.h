#pragma once
#include "MSPProcGenActorSelectable.h"

#include "ProceduralRulePackActor.generated.h"

class UMSPObjectFadeControllerComponent;

UCLASS(Abstract)
class AProceduralRulePackActor : public AMSPProcGenActorSelectable
{
	GENERATED_BODY()
public:

	AProceduralRulePackActor();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PRPActorSetupComplete();

	virtual void OnLayerPropertiesChanged();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnStartDespawn();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FString GetPOVInfo();
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString UICategoryName = "INVALID CATEGORY";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString UIDisplayName = "INVALID NAME";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString UIFormattedCoordinates = "INVALID COORDINATES";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Meta Data")
		TMap<FString, FString> ActorMetaData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UMSPObjectFadeControllerComponent* FadeController = nullptr;
private:
};
