#pragma once

#include "UObject/Object.h"
#include "ADisplayMethod.h"
#include "DisplayMethodNames.h"
#include "DMPointColour.generated.h"

UCLASS(BlueprintType)
class UDMPointColour : public UADisplayMethod
{
	GENERATED_BODY()

public:
    explicit UDMPointColour()
        : UADisplayMethod()
    {
    	method = DISPLAY_METHOD_POINT_COLOUR;
    }

	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Point colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString colour;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Point colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString sprite = "circle";
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Point colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		bool seabottom = false;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Point colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		float offset = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Point colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString material = "Unlit";

	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Point colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_colour;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Point colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_sprite;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Point colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_seabottom;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Point colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_offset;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Point colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_material;

	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Point colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_colour;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Point colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_sprite;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Point colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_seabottom;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Point colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_offset;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Point colour")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_material;
};
