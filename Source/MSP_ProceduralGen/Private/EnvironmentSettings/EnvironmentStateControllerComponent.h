#pragma once

#include "EnvironmentStateControllerComponent.generated.h"

class UEnvironmentStateSettings;

UINTERFACE(Blueprintable, MinimalAPI)
class UEnvironmentStateControllerComponent : public UInterface
{
	GENERATED_UINTERFACE_BODY()
public:
};

class IEnvironmentStateControllerComponent
{
	GENERATED_IINTERFACE_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void FindTargetComponents();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void UpdateTargetSettings(float DeltaSeconds, const UEnvironmentStateSettings* Settings);
};
