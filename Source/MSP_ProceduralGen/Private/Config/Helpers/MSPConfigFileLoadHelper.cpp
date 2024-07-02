#include "Config/Helpers/MSPConfigFileLoadHelper.h"

#include "Config/DisplayMethods/DMAreaColour.h"
#include "Config/DisplayMethods/DMAreaModelScatter.h"
#include "Config/DisplayMethods/DMBathymetry.h"
#include "Config/DisplayMethods/DMDensityFish.h"
#include "Config/DisplayMethods/DMDensityHeatMap.h"
#include "Config/DisplayMethods/DMLineColour.h"
#include "Config/DisplayMethods/DMLineModel.h"
#include "Config/DisplayMethods/DMLineModelScatter.h"
#include "Config/DisplayMethods/DMPointColour.h"
#include "Config/DisplayMethods/DMPointModel.h"
#include "Config/DisplayMethods/DMTypeMap.h"
#include "Config/Config.h"
#include "Config/DisplayMethods/ADisplayMethod.h"

namespace
{
	template <typename FmtType>
	FString FormatClassNameFromUObject_LoadHelper(const FmtType& a_fmt, const UClass* a_class)
	{
		const FString value(FString(a_class->GetPrefixCPP()) + a_class->GetName());
		return FString::Printf(a_fmt, *value);
	}

	UObject* CreateUObjectFromName(const FString& a_name)
	{
		if (a_name == FormatClassNameFromUObject_LoadHelper(TEXT("%s*"), UScaleConfig::StaticClass())) {
			return NewObject<UScaleConfig>();
		}
		if (a_name == FormatClassNameFromUObject_LoadHelper(TEXT("%s*"), UStringMap::StaticClass())) {
			return NewObject<UStringMap>();
		}
		if (a_name == FormatClassNameFromUObject_LoadHelper(TEXT("%s*"), UFloatArrayWrapper::StaticClass())) {
			return NewObject<UFloatArrayWrapper>();
		}
		if (a_name == FormatClassNameFromUObject_LoadHelper(TEXT("%s*"), UFloatArrayContainerWrapper::StaticClass())) {
			return NewObject<UFloatArrayContainerWrapper>();
		}
		checkf(false, *FString::Printf(TEXT("Unknown UObject: %s"), *a_name));
		return nullptr;
	}

	bool ManualSetObjectPropertyValues(const TSharedPtr<FJsonObject>& a_objectInput, const TSharedPtr<FJsonValue>& a_valueInput, UObject* a_object)
	{
		{
			UStringMap* o = dynamic_cast<UStringMap*>(a_object);
			if (o != nullptr) {
				for (auto item : a_objectInput->Values) {
					o->m_values.Add(item.Key, item.Value->AsString());
				}
				return true;
			}
		}
		{
			UFloatArrayContainerWrapper* o = dynamic_cast<UFloatArrayContainerWrapper*>(a_object);
			if (o != nullptr) {

				return true;
			}
		}
		{
			UFloatArrayWrapper* o = dynamic_cast<UFloatArrayWrapper*>(a_object);
			if (o != nullptr) {
				for (const auto& item : a_valueInput->AsArray()) {
					o->m_values.Add(item->AsNumber());
				}
				return true;
			}
		}
		return false;
	}

	void SetObjectPropertyValues(const TSharedPtr<FJsonObject>& a_objectInput, UObject* a_object)
	{
		for (TFieldIterator<FProperty> p(a_object->GetClass()); p; ++p) {
			FProperty* property = *p;
			if (property->IsA(FObjectProperty::StaticClass())) {
				UObject* object = CreateUObjectFromName(property->GetCPPType());
				if (nullptr == object) continue; // something went wrong, check the logs
				if (ManualSetObjectPropertyValues(a_objectInput->GetObjectField(property->GetName()), nullptr, object)) continue;
				CastField<FObjectProperty>(property)->SetObjectPropertyValue_InContainer(a_object, object);
				SetObjectPropertyValues(a_objectInput->GetObjectField(property->GetName()), object);
			}
			else if (property->IsA(FIntProperty::StaticClass())) {
				CastField<FIntProperty>(property)->SetPropertyValue_InContainer(a_object, a_objectInput->GetIntegerField(property->GetName()));
			}
			else if (property->IsA(FFloatProperty::StaticClass())) {
				CastField<FFloatProperty>(property)->SetPropertyValue_InContainer(a_object, a_objectInput->GetNumberField(property->GetName()));
			}
			else if (property->IsA(FStrProperty::StaticClass())) {
				CastField<FStrProperty>(property)->SetPropertyValue_InContainer(a_object, a_objectInput->GetStringField(property->GetName()));
			}
			else if (property->IsA(FBoolProperty::StaticClass())) {
				CastField<FBoolProperty>(property)->SetPropertyValue_InContainer(a_object, a_objectInput->GetBoolField(property->GetName()));
			}
			else if (property->IsA(FArrayProperty::StaticClass())) {
				TArray<TSharedPtr<FJsonValue>> arr = a_objectInput->GetArrayField(property->GetName());
				const FArrayProperty* arrayProperty = CastField<FArrayProperty>(property);
				void* arrayPropertyValue = arrayProperty->ContainerPtrToValuePtr<void>(a_object);
				TArray<UObject*>* arrayPtr = static_cast<TArray<UObject*>*>(arrayPropertyValue);
				for (const auto& el : arr) {
					UObject* object = CreateUObjectFromName(arrayProperty->Inner->GetCPPType());
					if (nullptr == object) continue; // something went wrong, check the logs
					if (ManualSetObjectPropertyValues(el->AsObject(), el, object)) {
						arrayPtr->Add(object);
						continue;
					}
					SetObjectPropertyValues(el->AsObject(), object);
					arrayPtr->Add(object);
				}
			}
			else {
				checkf(false, *FString::Printf(TEXT("Unknown property type: %s"), *property->GetCPPType()));
			}
		}
	}

	UStringMap* CreateStringMap(const TSharedPtr<FJsonObject>& a_stringMapInput)
	{
		UStringMap* result = NewObject<UStringMap>();
		for (auto pair : a_stringMapInput->Values)
		{
			result->m_values.Add(pair.Key, pair.Value->AsString());
		}
		return result;
	}

	UFloatArrayWrapper* CreateFloatArray(const TSharedPtr<FJsonValue>& a_floatArrayInput)
	{
		UFloatArrayWrapper* result = NewObject<UFloatArrayWrapper>();
		for (const auto& floatValue : a_floatArrayInput->AsArray())
		{
			result->m_values.Add(floatValue->AsNumber());
		}
		return result;
	}

	UFloatArrayContainerWrapper* CreateFloatArrayContainer(const TSharedPtr<FJsonValue>& a_floatArrayContainerInput)
	{
		UFloatArrayContainerWrapper* result = NewObject<UFloatArrayContainerWrapper>();
		for (const auto& floatArrayInput : a_floatArrayContainerInput->AsArray())
		{
			UFloatArrayWrapper* floatArray = NewObject<UFloatArrayWrapper>();
			for (const auto& number : floatArrayInput->AsArray()) {
				floatArray->m_values.Add(number->AsNumber());
			}
			result->m_values.Add(floatArray);
		}
		return result;
	}

	UADisplayMethod* CreateDisplayMethod(const TSharedPtr<FJsonObject>& a_displayMethodInput)
	{
		UADisplayMethod* result;

		const FString method = a_displayMethodInput->GetStringField("method");

		if (method == DISPLAY_METHOD_AREA_COLOUR) {
			result = NewObject<UDMAreaColour>();
		}
	    else if (method == DISPLAY_METHOD_AREA_MODEL_SCATTER) {
    		result = NewObject<UDMAreaModelScatter>();
	    }
	    else if (method == DISPLAY_METHOD_BATHYMETRY) {
			result = NewObject<UDMBathymetry>();;
	    }
	    else if (method == DISPLAY_METHOD_DENSITY_FISH) {
    		result = NewObject<UDMDensityFish>();
	    }
	    else if (method == DISPLAY_METHOD_DENSITY_HEAT_MAP) {
    		result = NewObject<UDMDensityHeatMap>();
	    }
	    else if (method == DISPLAY_METHOD_LINE_COLOUR) {
    		result = NewObject<UDMLineColour>();
	    }
	    else if (method == DISPLAY_METHOD_LINE_MODEL) {
    		result = NewObject<UDMLineModel>();
	    }
	    else if (method == DISPLAY_METHOD_LINE_MODEL_SCATTER) {
    		result = NewObject<UDMLineModelScatter>();
	    }
	    else if (method == DISPLAY_METHOD_POINT_COLOUR) {
    		result = NewObject<UDMPointColour>();
	    }
	    else if (method == DISPLAY_METHOD_POINT_MODEL) {
    		result = NewObject<UDMPointModel>();
	    }
	    else {
			checkf(false, *FString::Printf(TEXT("Unknown display method: %s"), *method));
			return nullptr;
		}

		SetObjectPropertyValues(a_displayMethodInput, result);

		return result;
	}

	void ManualJsonObjectToALayer(const TSharedPtr<FJsonObject>& a_jsonObject, UALayer* a_layerInstance)
	{
		//TArray<TSharedPtr<FJsonValue>> displayMethodInputContainer = a_jsonObject->GetArrayField("display_methods");
		//for (const auto& displayMethodInput : displayMethodInputContainer) {
		//	UADisplayMethod* displayMethod = CreateDisplayMethod(displayMethodInput->AsObject());
		//	if (nullptr == displayMethod) { // something went wrong, check the logs
		//		continue;
		//	}
		//	a_layerInstance->display_methods.Add(displayMethod);
		//}
		verify(a_layerInstance->types.IsEmpty());
		TArray<TSharedPtr<FJsonValue>> typesInputContainer = a_jsonObject->GetArrayField("types");
		for (const auto& typesInput : typesInputContainer) {
			a_layerInstance->types.Add(CreateStringMap(typesInput->AsObject()));
		}
	}

	void ManualJsonObjectToUConfigFile(const TSharedPtr<FJsonObject>& a_jsonObject, const UConfig* a_instance)
	{
		const TSharedPtr<FJsonObject> dataModel = a_jsonObject->GetObjectField("datamodel");

		TArray<TSharedPtr<FJsonValue>> rasterLayerInputContainer = dataModel->GetArrayField("raster_layers");
		for (int32 index = 0; index < rasterLayerInputContainer.Num(); ++index) {
			const auto& rasterLayerInput = rasterLayerInputContainer[index];
			ManualJsonObjectToALayer(rasterLayerInput->AsObject(), a_instance->datamodel->raster_layers[index]);
		}

		TArray<TSharedPtr<FJsonValue>> vectorLayerInputContainer = dataModel->GetArrayField("vector_layers");
		for (int32 index = 0; index < vectorLayerInputContainer.Num(); ++index) {
			const auto& vectorLayerInput = vectorLayerInputContainer[index];
			ManualJsonObjectToALayer(vectorLayerInput->AsObject(), a_instance->datamodel->vector_layers[index]);

			TArray<TSharedPtr<FJsonValue>> vectorLayerDataInputContainer = vectorLayerInput->AsObject()->GetArrayField("data");
			for (int32 j = 0; j < vectorLayerDataInputContainer.Num(); ++j) {
				const auto& vectorLayerDataInput = vectorLayerDataInputContainer[j];
				a_instance->datamodel->vector_layers[index]->data[j]->metadata = CreateStringMap(vectorLayerDataInput->AsObject()->GetObjectField("metadata"));

				for (const auto& pointInput : vectorLayerDataInput->AsObject()->GetArrayField("points")) {
					UFloatArrayWrapper* point = CreateFloatArray(pointInput);
					a_instance->datamodel->vector_layers[index]->data[j]->points.Add(point);
				}
				for (const auto& gapsInput : vectorLayerDataInput->AsObject()->GetArrayField("gaps")) {
					UFloatArrayContainerWrapper* gaps = CreateFloatArrayContainer(gapsInput);
					a_instance->datamodel->vector_layers[index]->data[j]->gaps.Add(gaps);
				}
			}
		}

		// POV expects the Bathmetry scaling min and max to be different, so adjust it
		URasterLayer** bathymetryLayer = a_instance->datamodel->raster_layers.FindByPredicate([](const URasterLayer* a_rasterLayer){
			 // to identify the Bathymetry raster layer, we check for the tags "Bathymetry" and "ValueMap"
			return a_rasterLayer->tags.Contains("Bathymetry") && a_rasterLayer->tags.Contains("ValueMap");
		});
		if (!bathymetryLayer) return;
		(*bathymetryLayer)->scale.min_value *= -UConfig::MetersToUnrealUnits; // from meter to centimeters
		(*bathymetryLayer)->scale.max_value *= -UConfig::MetersToUnrealUnits;
		for (auto& [normalised_input_value, min_output_value] : (*bathymetryLayer)->scale.groups) {
			min_output_value *= -UConfig::MetersToUnrealUnits;
		}
	}
}

void FMSPConfigFileLoadHelper::OnPostConvertJsonObjectToClass(TSharedPtr<FJsonObject> a_jsonObject, UConfig* a_result)
{
	ManualJsonObjectToUConfigFile(a_jsonObject, a_result);
}
