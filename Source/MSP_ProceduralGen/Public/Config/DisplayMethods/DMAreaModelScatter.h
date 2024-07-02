#pragma once

#include "UObject/Object.h"
#include "ADisplayMethod.h"
#include "DisplayMethodNames.h"
#include "Config/VectorLayer.h"
#include "Config/VectorObject.h"
#include "DMAreaModelScatter.generated.h"

UCLASS(BlueprintType)
class UDMAreaModelScatter : public UADisplayMethod
{
	GENERATED_BODY()

public:
    explicit UDMAreaModelScatter()
        : UADisplayMethod()
    {
    	method = DISPLAY_METHOD_AREA_MODEL_SCATTER;
    }

	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString model;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		int32 amount;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		float spacing;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		bool seabottom = false;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		float offset = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString material = "Unlit";

	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_model;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_amount;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_spacing;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_seabottom;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_offset;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString type_material;

	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_model;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_amount;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_spacing;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_seabottom;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_offset;
	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Area model scatter")
		// ReSharper disable once CppInconsistentNaming, need to map with the json
		FString meta_material;

	UFUNCTION(BlueprintCallable, Category = "Config|Data model|Layer|Display method|Area model scatter", DisplayName="Get parent vector layer")
		bool GetParentVectorLayer(UVectorLayer*& a_vectorLayer) const;
	UFUNCTION(BlueprintCallable, Category = "Config|Data model|Layer|Display method|Area model scatter", DisplayName="Get the name of the model being placed")
		bool GetModelByDataIndex(int32 a_dataIndex, FString& a_model) const;
	UFUNCTION(BlueprintCallable, Category = "Config|Data model|Layer|Display method|Area model scatter", DisplayName="Get the amount of models to scatter")
		bool GetAmountByDataIndex(int32 a_dataIndex, int32& a_amount) const;
	UFUNCTION(BlueprintCallable, Category = "Config|Data model|Layer|Display method|Area model scatter", DisplayName="Get the amount of spacing between models, in world scale")
		bool GetWorldSpacingByDataIndex(int32 a_dataIndex, float& a_spacing) const;
	UFUNCTION(BlueprintCallable, Category = "Config|Data model|Layer|Display method|Area model scatter", DisplayName="Get seabottom value. If 'false' the object is aligned to the surface, otherwise it is aligned to the sea bottom.")
		bool GetSeaBottomByDataIndex(int32 a_dataIndex, bool& a_seaBottom) const;
	UFUNCTION(BlueprintCallable, Category = "Config|Data model|Layer|Display method|Area model scatter", DisplayName="Get the material the model is rendered with.")
		bool GetMaterialByDataIndex(int32 a_dataIndex, FString& a_material) const;

private:
	// A single parameter can be defined on multiple levels, with values being overridden in a fixed order: metadata → type data → Display Method (fixed) data.
	// For example, a Display Method could contain "model", "type_model" and "meta_model",
	//   in which case the value of "meta_model" would used first if it is defined, "type_model" next and otherwise "model".
	template <typename FieldValueType>
	bool GetFieldValueByDataIndex(
		int32 a_dataIndex,
		const FString& a_metaFieldValueRef,
		const FString& a_typeFieldValueRef,
		const FieldValueType a_fixedFieldValue,
		FieldValueType& a_fieldValue) const
	{
		check(false);
		return false;

		//if (a_metaFieldValueRef.IsEmpty() && a_typeFieldValueRef.IsEmpty()) {
		//	a_fieldValue = a_fixedFieldValue;
		//	return true;
		//}
		//UVectorLayer* parentVectorLayer;
		//if (!GetParentVectorLayer(parentVectorLayer)) {
		//	return false;
		//}
		//if (a_dataIndex >= parentVectorLayer->data.Num()) {
		//	UE_LOG(LogTemp, Error, TEXT("%hs: Encountered invalid data index: %d. Expected less than %d"), __FUNCTION__, a_dataIndex, parentVectorLayer->data.Num());
		//	return false;
		//}
		//const UVectorObject* data = parentVectorLayer->data[a_dataIndex];
		//if (!a_metaFieldValueRef.IsEmpty()) {
		//    if constexpr (std::is_same_v<FieldValueType, int32>)
		//    {
		//        a_fieldValue = data->GetMetadataValueAsInt(a_metaFieldValueRef);
		//    }
		//    else if constexpr (std::is_same_v<FieldValueType, float>)
		//    {
		//        a_fieldValue = data->GetMetadataValueAsFloat(a_metaFieldValueRef);
		//    }
		//    else if constexpr (std::is_same_v<FieldValueType, FString>)
		//    {
		//        a_fieldValue = data->GetMetadataValue(a_metaFieldValueRef);
		//    }
		//    else // bool
		//    {
		//        a_fieldValue = data->GetMetadataValueAsBool(a_metaFieldValueRef);
		//    }
		//	return true;
		//}
		//if (!a_metaFieldValueRef.IsEmpty() && data->types.Num() > 0 && data->types[0] < parentVectorLayer->types.Num()) {
		//	const UStringMap* stringMap = parentVectorLayer->types[data->types[0]];
		//	if (!stringMap->m_values.Contains(a_metaFieldValueRef)) {
		//		return false; // reference does not exist.
		//	}
		//    if constexpr (std::is_same_v<FieldValueType, int32>)
		//    {
		//        a_fieldValue = FCString::Atoi(*stringMap->m_values[a_metaFieldValueRef]);
		//    }
		//    else if constexpr (std::is_same_v<FieldValueType, float>)
		//    {
		//        a_fieldValue = FCString::Atof(*stringMap->m_values[a_metaFieldValueRef]);
		//    }
		//    else if constexpr (std::is_same_v<FieldValueType, FString>)
		//    {
		//        a_fieldValue = stringMap->m_values[a_metaFieldValueRef];
		//    }
		//    else // bool
		//    {
		//        a_fieldValue = stringMap->m_values[a_metaFieldValueRef] == "true";
		//    }
		//	return true;
		//}
		//// there is no amount.
		//return false;
	}
};
