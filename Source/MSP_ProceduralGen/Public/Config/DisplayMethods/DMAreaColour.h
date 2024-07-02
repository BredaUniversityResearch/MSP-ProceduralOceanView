#pragma once

#include "UObject/Object.h"
#include "ADisplayMethod.h"
#include "DisplayMethodNames.h"
#include "DMAreaColour.generated.h"

UCLASS(BlueprintType)
class UDMAreaColour : public UADisplayMethod
{
	GENERATED_BODY()

public:
    explicit UDMAreaColour()
        : UADisplayMethod()
    {
    	method = DISPLAY_METHOD_AREA_COLOUR;
    }

	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString fill_colour;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString outline_colour;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		float outline_scale = 1;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		bool seabottom = false;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		float offset = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString material = "Unlit";

	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_fill_colour;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_outline_colour;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_outline_scale;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_seabottom;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_offset;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_material;

	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_fill_colour;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_outline_colour;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_outline_scale;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_seabottom;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_offset;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_material;
};