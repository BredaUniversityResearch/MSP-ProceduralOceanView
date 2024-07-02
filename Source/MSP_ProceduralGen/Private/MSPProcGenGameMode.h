// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MSPProcGenGameMode.generated.h"

class UGeoCoordinateSystemTransformation;
class UConfig;

USTRUCT(BlueprintType)
struct FMSPSimulationProjectionBounds
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FBox2D SimulatedBounds;
	UPROPERTY(BlueprintReadOnly)
	FVector2D WorldOffset;
	//UPROPERTY(BlueprintReadOnly)
	//FString ProjectionIdentifier;
};

class UProceduralRulePackManager;

UCLASS()
class AMSPProcGenGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AMSPProcGenGameMode(const FObjectInitializer& Initializer);

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(meta=(DeprecatedFunction, DeprecationMessage="Config should be not be kept around"))
	UConfig* GetLoadedConfig() const;

	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly)
	UProceduralRulePackManager* PRPManager{nullptr};

	UPROPERTY(BlueprintReadOnly)
	FMSPSimulationProjectionBounds SimulationProjection;

	UPROPERTY()
	UGeoCoordinateSystemTransformation* LoadedConfigToLatLon;

private:
	UPROPERTY(EditDefaultsOnly)
  	TSoftObjectPtr<UWorld> StandAloneSubLevel {nullptr};
  	UPROPERTY(EditDefaultsOnly)
  	TSoftObjectPtr<UWorld> VRSubLevel{nullptr};

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APawn> VRPawnClass{nullptr};

	UPROPERTY(Transient)
	UConfig* LoadedConfig{nullptr};
  };
