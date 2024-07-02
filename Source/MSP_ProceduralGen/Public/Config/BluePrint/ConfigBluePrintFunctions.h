#pragma once

#include "ConfigBluePrintFunctions.generated.h"

UCLASS(Blueprintable)
class MSP_PROCEDURALGEN_API UConfigBluePrintFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, category = "Config", DisplayName="Query config by json path")
	// ReSharper disable CppInconsistentNaming
	static bool Query(UObject* a_config, const FString& a_query, TArray<UObject*>& a_objectArray, UObject*& a_object, TArray<float>& a_floatsArray, float& a_float, TArray<int32>& a_intArray, int32& a_int, TMap<FString, FString>& a_stringMap, TArray<FString>& a_stringArray, FString& a_string, bool& a_boolean, FString& a_outputType, FString& a_error);

	UFUNCTION(BlueprintCallable, category = "Config", DisplayName="Config base path directory")
	static FString GetJsonBasePathFromDir()
	{
		return BASE_PATH_FROM_CONTENT_DIR;
	}

	UFUNCTION(BlueprintCallable, category = "Config", DisplayName="Config file name")
	static FString GetJsonConfigFilename()
	{
		return CONFIG_FILENAME;
	}

private:
	inline static const FString BASE_PATH_FROM_CONTENT_DIR = "/MSP_ProceduralOceanView/08_NonAssets/";
	inline static const FString CONFIG_FILENAME = "pov-config.json";
};
