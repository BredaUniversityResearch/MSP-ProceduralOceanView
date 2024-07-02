#pragma once

#include "UObject/Object.h"
#include "ADisplayMethod.h"
#include "DisplayMethodNames.h"
#include "DMLineColour.generated.h"

UCLASS(BlueprintType)
class UDMLineColour : public UADisplayMethod
{
	GENERATED_BODY()

public:
    explicit UDMLineColour()
        : UADisplayMethod()
    {
    	method = DISPLAY_METHOD_LINE_COLOUR;
    }

	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString colour;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		float width = -1;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString direction = "None"; /* None, Forward, Backward, Both */
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		bool seabottom = false;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		float offset = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString material = "Unlit";

	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_colour;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_width;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_direction;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_seabottom;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_offset;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_material;

	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_colour;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_width;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_direction;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_seabottom;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_offset;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Line colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_material;
};
