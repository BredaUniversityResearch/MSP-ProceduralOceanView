#pragma once

#include "MSPProcGenActor.h"
#include "MSPProcGenActorSelectable.generated.h"

UCLASS(BlueprintType, Blueprintable)
class AMSPProcGenActorSelectable : public AMSPProcGenActor
{
	GENERATED_BODY()

public:
	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnSelected();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnDoubleClickedEvent();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnDeselected();
	
};
