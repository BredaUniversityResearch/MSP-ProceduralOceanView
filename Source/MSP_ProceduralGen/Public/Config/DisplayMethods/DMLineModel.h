#pragma once

#include "UObject/Object.h"
#include "ADisplayMethod.h"
#include "DisplayMethodNames.h"
#include "DMLineModel.generated.h"

UCLASS(BlueprintType)
class UDMLineModel : public UADisplayMethod
{
	GENERATED_BODY()

public:
    explicit UDMLineModel()
        : UADisplayMethod()
    {
    	method = DISPLAY_METHOD_LINE_MODEL;
    }

	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString model;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		int32 amount = 1;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		bool seabottom = false;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		float offset = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString material = "Unlit";

	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_model;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_amount;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_seabottom;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_offset;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_material;

	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_model;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_amount;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_seabottom;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_offset;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line model")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_material;
};
