// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "MSPProcGenGameInstance.generated.h"

class UConfig;
class UGenerationSettings;
class UDataPool;

/**
 *
 */
UCLASS()
class MSP_PROCEDURALGEN_API UMSPProcGenGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;

	static UMSPProcGenGameInstance* GetInstance();

	UDataPool& GetDataPool() const;
	void ResetDataPool();

	UFUNCTION()
	virtual void BeginLoadingScreen(const FString& MapName);

	UFUNCTION()
	virtual void EndLoadingScreen(UWorld* InLoadedWorld);

	UFUNCTION(BlueprintCallable, Category = "Generation Settings")
	UGenerationSettings* GetGenerationSettings();

private:
	UPROPERTY(EditAnywhere, Category = "Generation Settings")
	UGenerationSettings* m_generationSettings;
	UPROPERTY()
	UDataPool* m_dataPool;

	static UMSPProcGenGameInstance* ms_instance;
};
