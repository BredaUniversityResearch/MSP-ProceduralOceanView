// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GenerationSettings.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class MSP_PROCEDURALGEN_API UGenerationSettings : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics Settings")
	bool IsVR = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Settings")
	FString JsonFilePath = "DE Riffgat/pov-config.json";
};

USTRUCT(BlueprintType)
struct FPropertyStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Settings")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Settings")
	FString Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation Settings")
	FString Value;

};

//func lib
UCLASS()
class MSP_PROCEDURALGEN_API UGenerationSettingsFuncLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Generation Settings")
	static void GetGenerationSettingsPropertyArray(TArray<FPropertyStruct>& OutProperties);

	UFUNCTION(BlueprintCallable, Category = "Generation Settings")
	static void SetPropertyGenerationSettings(UGenerationSettings* Container, const FPropertyStruct& PropertyStruct, FString Value);

	UFUNCTION(BlueprintCallable, Category = "Generation Settings")
	static void GetPropertyGenerationSettings(UGenerationSettings* Container, const FPropertyStruct& PropertyStruct, FString& Value);

	UFUNCTION(BlueprintCallable, Category = "Generation Settings")
	static bool CallFunctionByName(UObject* ObjPtr, FName FunctionName);


};

