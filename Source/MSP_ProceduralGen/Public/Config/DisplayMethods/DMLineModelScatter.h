#pragma once

#include "UObject/Object.h"
#include "ADisplayMethod.h"
#include "DisplayMethodNames.h"
#include "DMLineModelScatter.generated.h"

UCLASS(BlueprintType)
class UDMLineModelScatter : public UADisplayMethod
{
	GENERATED_BODY()

public:
    explicit UDMLineModelScatter()
        : UADisplayMethod()
    {
    	method = DISPLAY_METHOD_LINE_MODEL_SCATTER;
    }

	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		TArray<FString> models;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		int32 amount = 1;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		float spacing;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString direction = "None";
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		bool seabottom = false;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		float offset = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString material = "Unlit";

	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_models;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_amount;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_spacing;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_direction;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_seabottom;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_offset;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_material;

	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_models;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_amount;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_spacing;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_direction;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_seabottom;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_offset;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_material;
};
