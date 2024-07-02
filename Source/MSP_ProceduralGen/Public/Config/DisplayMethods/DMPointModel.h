#pragma once

#include "UObject/Object.h"
#include "ADisplayMethod.h"
#include "DisplayMethodNames.h"
#include "DMPointModel.generated.h"

UCLASS(BlueprintType)
class UDMPointModel : public UADisplayMethod
{
	GENERATED_BODY()

public:
    explicit UDMPointModel()
        : UADisplayMethod()
    {
    	method = DISPLAY_METHOD_POINT_MODEL;
    }

	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Point model")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString model;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Point model")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		bool seabottom = false;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Point model")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		float offset = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Point model")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString material = "Unlit";

	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Point model")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_model;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Point model")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_seabottom;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Point model")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_offset;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Point model")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_material;

	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Point model")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_model;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Point model")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_seabottom;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Point model")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_offset;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Point model")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_material;
};
