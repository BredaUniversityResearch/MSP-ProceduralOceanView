#include "Config/BluePrint/ConfigBluePrintFunctions.h"

#include "Config/Config.h"
#include "Config/Common/FloatArrayContainerWrapper.h"
#include "Config/Common/FloatArrayWrapper.h"
#include "Config/Common/StringMap.h"

namespace // unnamed namespace
{
	template <typename FmtType>
	FString FormatClassNameFromUObject_Config(const FmtType& a_fmt, const UClass* a_class)
	{
		const FString value(FString(a_class->GetPrefixCPP()) + a_class->GetName());
		return FString::Printf(a_fmt, *value);
	}

	FProperty* FindProperty(const UObject* a_parent, const FString& a_propertyName)
	{
		for (TFieldIterator<FProperty> p(a_parent->GetClass()); p; ++p) {
			FProperty* property = *p;
			if (property->GetName() == a_propertyName) {
				return property;
			}
		}
		return nullptr;
	}

	void ExportPropertyValue(const UObject* a_parent, FProperty* a_property, TArray<UObject*>& a_objectArray, UObject*& a_object, TArray<float>& a_floatsArray, float& a_float, TArray<int32>& a_IntArray, int32& a_int, TMap<FString, FString>& a_stringMap, TArray<FString>& a_stringArray, FString& a_String, bool& a_boolean, FString& a_outputType)
	{
		TArray<uint8> propertyValue;
		propertyValue.AddZeroed(a_property->GetSize());
		a_property->GetValue_InContainer(a_parent, propertyValue.GetData());

		if (a_property->IsA(FMapProperty::StaticClass())) {
			a_stringMap = *reinterpret_cast<TMap<FString,FString>*>(propertyValue.GetData());
			a_outputType = "class TMap<FString, FString>";
			return;
		}

		if (a_property->IsA(FArrayProperty::StaticClass())) {
			const FArrayProperty* arrayProperty = CastField<FArrayProperty>(a_property);
			if (arrayProperty->Inner->IsA(FIntProperty::StaticClass())) {
				a_IntArray = *reinterpret_cast<TArray<int32>*>(propertyValue.GetData());
				a_outputType = "class TArray<int32>";
			}
			else if (arrayProperty->Inner->IsA(FFloatProperty::StaticClass())) {
				a_floatsArray = *reinterpret_cast<TArray<float>*>(propertyValue.GetData());
				a_outputType = "class TArray<float>";
			}
			else if (arrayProperty->Inner->IsA(FStrProperty::StaticClass())) {
				a_stringArray = *reinterpret_cast<TArray<FString>*>(propertyValue.GetData());
				a_outputType = "class TArray<FString>";
			}
			else if (arrayProperty->Inner->IsA(FObjectProperty::StaticClass())) {
				a_objectArray = *reinterpret_cast<TArray<UObject*>*>(propertyValue.GetData());
				a_outputType = "class TArray<" + arrayProperty->Inner->GetCPPType() + ">";
			}
			return;
		}

		if (a_property->IsA(FIntProperty::StaticClass())) {
			a_int = *reinterpret_cast<int32*>(propertyValue.GetData());
			a_outputType = "int";
		}
		else if (a_property->IsA(FFloatProperty::StaticClass())) {
			a_float = *reinterpret_cast<float*>(propertyValue.GetData());
			a_outputType = "float";
		}
		else if (a_property->IsA(FStrProperty::StaticClass())) {
			a_String = *reinterpret_cast<FString*>(propertyValue.GetData());
			a_outputType = "class FString";
		}
		else if (a_property->IsA(FBoolProperty::StaticClass())) {
			a_boolean = *reinterpret_cast<bool*>(propertyValue.GetData());
			a_outputType = "bool";
		}
		else if (a_property->IsA(FObjectProperty::StaticClass())) {
			const FObjectProperty* objProperty = CastField<FObjectProperty>(a_property);
			a_object = objProperty->GetObjectPropertyValue_InContainer(a_parent);
			a_outputType = objProperty->GetCPPTypeForwardDeclaration();
			a_outputType[a_outputType.Len()-1]='*';
		}
	}

	TArray<UObject*> GetUObjectsFromArrayByPredicate(const FString& a_predicate, TArray<UObject*>& a_objectArray)
	{
		TArray<UObject*> predMatches;
		FString propertyNameBase;
		FString propertyValue;
		if (!a_predicate.Split("=", &propertyNameBase, &propertyValue, ESearchCase::IgnoreCase, ESearchDir::FromEnd)) {
			return predMatches;
		}

		TArray<FString> propertyNames;
		propertyNameBase.ParseIntoArray(propertyNames, TEXT("."));

		TMap<UObject*,UObject*> objectMap;
		for (auto o : a_objectArray) {
			objectMap.Add(o, o);
		}
		while (propertyNames.Num() > 0) {
			FString propertyName = propertyNames[0];
			TMap<UObject*,UObject*> newObjectMap;
			for(const auto& opair : objectMap) {
				FProperty* property = FindProperty(opair.Value, propertyName);
				if (property == nullptr) {
					continue;
				}
				TArray<UObject*> os; UObject* o; TArray<float> floatsArray; float f; TArray<int32> intArray; int32 i; TMap<FString, FString> stringMap; TArray<FString> stringArray; FString string; bool boolean; FString outputType;
				ExportPropertyValue(opair.Value, property, os, o, floatsArray, f, intArray, i, stringMap, stringArray, string, boolean, outputType);

				if (propertyNames.Num() == 1) {
					FRegexMatcher matcher(FRegexPattern(propertyValue), string);
					if (matcher.FindNext()) {
						predMatches.Add(opair.Key);
						continue;
					}

					if (propertyValue == string || propertyValue == FString::FromInt(i) || propertyValue == (boolean == true ? "true" : "false") ||
						(outputType == "float" && FMath::IsNearlyZero(FCString::Atof(*propertyValue) - f))) {
						predMatches.Add(opair.Key);
					}
				} else {
					for (auto oo : os) {
						newObjectMap.Add(opair.Key, oo);
					}
				}
			}
			objectMap = newObjectMap;
			propertyNames.RemoveAt(0);
		}
		return predMatches;
	}

	bool PostFixPartsStringMap(TArray<FString> a_parts, TArray<UObject*>& a_objectArray, UObject*& a_object, TArray<float>& a_floatsArray, float& a_float, TArray<int32>& a_intArray, int32& a_int, TMap<FString, FString>& a_stringMap, TArray<FString>& a_stringArray, FString& a_string, bool& a_boolean, FString& a_outputType)
	{
		if (!(a_outputType == "class TMap<FString, FString>" ||
			a_outputType == FormatClassNameFromUObject_Config(TEXT("class %s*"), UStringMap::StaticClass()))) { // handle stringMap only
			return false;
		}

		if (a_outputType == FormatClassNameFromUObject_Config(TEXT("class %s*"), UStringMap::StaticClass())) {
			const UStringMap* stringMap = static_cast<UStringMap*>(a_object);
			a_stringMap = stringMap->m_values;
		}

		if (a_parts.Num() > 0) { // handle to string value
			const FString part = a_parts[0];
			if (a_stringMap.Contains(part)) {
				a_string = a_stringMap[part];
				a_outputType = "class FString";
			}
		}

		return true;
	}

	bool PostFixArrayStringMap(TArray<FString> a_parts, const FString& a_predicate, int32 a_arrayIndex, TArray<UObject*>& a_objectArray, UObject*& a_object, TArray<float>& a_floatsArray, float& a_float, TArray<int32>& a_intArray, int32& a_int, TMap<FString, FString>& a_stringMap, TArray<FString>& a_stringArray, FString& a_string, bool& a_boolean, FString& a_outputType)
	{
		if (a_outputType != FormatClassNameFromUObject_Config(TEXT("class TArray<%s*>"), UStringMap::StaticClass())) {
			return false;
		}

		FString propertyName;
		FString propertyValue;
		a_object = nullptr;
		if (a_predicate.Split("=", &propertyName, &propertyValue, ESearchCase::IgnoreCase, ESearchDir::FromEnd)) {
			for (const auto& o : a_objectArray) {
				UStringMap* stringMap = static_cast<UStringMap*>(o);
				if (!stringMap->m_values.Contains(propertyName)) {
					continue;
				}
				if (stringMap->m_values[propertyName] != propertyValue) {
					continue;
				}
				a_object = stringMap;
				a_stringMap = stringMap->m_values;
				a_outputType = "class TMap<FString, FString>";
				PostFixPartsStringMap(a_parts, a_objectArray, a_object, a_floatsArray, a_float, a_intArray, a_int, a_stringMap, a_stringArray, a_string, a_boolean, a_outputType);
			}
		}
		else if (a_arrayIndex != -1) {
			a_object = a_objectArray[a_arrayIndex];
		}

		if (a_object != nullptr) {
			UStringMap* stringMap = static_cast<UStringMap*>(a_object);
			a_stringMap = stringMap->m_values;
			a_outputType = "class TMap<FString, FString>";
			PostFixPartsStringMap(a_parts, a_objectArray, a_object, a_floatsArray, a_float, a_intArray, a_int, a_stringMap, a_stringArray, a_string, a_boolean, a_outputType);
		}

		return true;
	}

	bool PostFixArrayFloatArray(TArray<FString> a_parts, const FString& a_predicate, int32 a_arrayIndex, TArray<UObject*>& a_objectArray, UObject*& a_object, TArray<float>& a_floatsArray, float& a_float, TArray<int32>& a_intArray, int32& a_int, TMap<FString, FString>& a_stringMap, TArray<FString>& a_stringArray, FString& a_string, bool& a_boolean, FString& a_outputType)
	{
		if (a_outputType != FormatClassNameFromUObject_Config(TEXT("class TArray<%s*>"), UFloatArrayWrapper::StaticClass())) { // handle array of floatArray object only
			return false;
		}
		if (a_arrayIndex == -1) {
			return true;
		}
		a_object = a_objectArray[a_arrayIndex];
		const UFloatArrayWrapper* floatArray = Cast<UFloatArrayWrapper>(a_object);
		a_floatsArray = floatArray->m_values;
		a_outputType = "class TArray<float>";
		return true;
	}

	void PostFix(TArray<FString> a_parts, const FString& a_predicate, int32 a_arrayIndex, TArray<UObject*>& a_objectArray, UObject*& a_object, TArray<float>& a_floatsArray, float& a_float, TArray<int32>& a_intArray, int32& a_int, TMap<FString, FString>& a_stringMap, TArray<FString>& a_stringArray, FString& a_string, bool& a_boolean, FString& a_outputType)
	{
		if (PostFixPartsStringMap(a_parts, a_objectArray, a_object, a_floatsArray, a_float, a_intArray, a_int, a_stringMap, a_stringArray, a_string, a_boolean, a_outputType)) {
			return;
		}
		if (PostFixArrayStringMap(a_parts, a_predicate, a_arrayIndex, a_objectArray, a_object, a_floatsArray, a_float, a_intArray, a_int, a_stringMap, a_stringArray, a_string, a_boolean, a_outputType)) {
			return;
		}
		if (PostFixArrayFloatArray(a_parts, a_predicate, a_arrayIndex, a_objectArray, a_object, a_floatsArray, a_float, a_intArray, a_int, a_stringMap, a_stringArray, a_string, a_boolean, a_outputType)) {
			return;
		}
	}

	bool DoQuery(UObject* a_config, const FString& a_query, TArray<UObject*>& a_objectArray, UObject*& a_object, TArray<float>& a_floatsArray, float& a_float, TArray<int32>& a_intArray, int32& a_int, TMap<FString, FString>& a_stringMap, TArray<FString>& a_stringArray, FString& a_string, bool& a_boolean, FString& a_outputType, FString& a_error)
	{
		if (a_config == nullptr) {
			UE_LOG(LogTemp, Error, TEXT("Config is not loaded"));
			a_error = "Config is not loaded";
			return false;
		}

		// get all parts.
		TArray<FString> parts;
		FRegexMatcher matcher(FRegexPattern("[^\\.\\[]+(?:\\[.+?\\])?"), a_query);
		int32 captureIndex = 0;
		while (matcher.FindNext())
		{
		    int32 beginning = matcher.GetMatchBeginning();
		    int32 ending = matcher.GetMatchEnding();
		    FString match = a_query.Mid(beginning, ending - beginning);
			parts.Add(match);
		    captureIndex++;
		}

		UObject* parent = a_config;
		FString predicate;
		int32 arrayIndex = -1;
		while (parent != nullptr && parts.Num() > 0) {
			FString part = parts[0];
			parts.RemoveAt(0);

			FString leftSubstring;
			FString rightSubstring;
			predicate = "";
			arrayIndex = -1;
			if (part.Split("[", &leftSubstring, &rightSubstring, ESearchCase::IgnoreCase, ESearchDir::FromEnd)) {
				part = leftSubstring;
				predicate = rightSubstring.Mid(0, rightSubstring.Len() - 1);
				if (predicate.IsNumeric()) {
					arrayIndex = FCString::Atoi(*predicate);
					predicate = "";
				}
			}

			FProperty* property = FindProperty(parent, part);
			if (property == nullptr) {
				UE_LOG(LogTemp, Error, TEXT("Unknown property: %s"), *part);
				a_error = "Unknown property: " + part;
				return false; // something went wrong.
			}
			a_objectArray.Empty(); a_object = nullptr; a_floatsArray.Empty(); a_float = 0.0f; a_intArray.Empty(); a_int = 0; a_stringMap.Empty(); a_string = ""; a_boolean = false; a_outputType = "";
			ExportPropertyValue(parent, property, a_objectArray, a_object, a_floatsArray, a_float, a_intArray, a_int, a_stringMap, a_stringArray, a_string, a_boolean, a_outputType);
			if (a_outputType == FormatClassNameFromUObject_Config(TEXT("class %s*"), UStringMap::StaticClass()) ||
				a_outputType == FormatClassNameFromUObject_Config(TEXT("class TArray<%s*>"), UStringMap::StaticClass()) ||
				a_outputType == FormatClassNameFromUObject_Config(TEXT("class TArray<%s*>"), UFloatArrayWrapper::StaticClass())) {
				parent = nullptr;
				continue;
			}
			if (a_objectArray.Num() > 0) {
				if (predicate.Len() > 0) {
					TArray<UObject*> objects = GetUObjectsFromArrayByPredicate(predicate, a_objectArray);
					if (objects.Num() == 0) {
						UE_LOG(LogTemp, Error, TEXT("Could not match array predicate: %s"), *predicate);
						a_error = "Could not match array predicate: " + predicate;
					}
					else if (objects.Num() == 1) {
						parent = objects[0];
						a_object = parent;
						a_objectArray.Empty(); // such that you can always use the array if the user is unsure about single or multiple results.
						a_objectArray.Add(a_object);
						a_outputType = "class " + CastField<FArrayProperty>(property)->Inner->GetCPPType();
					}
					else {
						a_objectArray = objects;
					}
					continue;
				}
				parent = a_objectArray[arrayIndex == -1 ? 0 : arrayIndex];
				continue;
			}
			if (a_object != nullptr) {
				parent = a_object;
				continue;
			}

			if (parts.Num() > 0) {
				UE_LOG(LogTemp, Error, TEXT("Could not resolve the next part: %s"), *parts[0]);
				a_error = "Could not resolve the next part: " + parts[0];
			}
			break;
		}
		PostFix(parts, predicate, arrayIndex, a_objectArray, a_object, a_floatsArray, a_float, a_intArray, a_int, a_stringMap, a_stringArray, a_string, a_boolean, a_outputType);
		UE_LOG(LogTemp, Display, TEXT("Query: %s. Output type: %s"), *a_query, *a_outputType);
		return true;
	}

	void UnitTest1(UObject* a_config)
	{
		TArray<UObject*> objectArray; UObject* o; TArray<float> floatsArray; float f; TArray<int32> intArray; int32 i; TMap<FString, FString> stringMap; TArray<FString> stringArray; FString string; bool boolean; FString outputType; FString error;
		const bool result = DoQuery(a_config, "metadata.date_modified", objectArray, o, floatsArray, f, intArray, i, stringMap, stringArray, string, boolean, outputType, error);
		checkf(result, TEXT("query failed"));
		checkf(outputType == "class FString", TEXT("Encountered unexpected output type"));
		checkf(string == "24/05/2022", TEXT("Encountered unexpected value"));
	}

	void UnitTest2(UObject* a_config)
	{
		TArray<UObject*> objectArray; UObject* o; TArray<float> floatsArray; float f; TArray<int32> intArray; int32 i = -1; TMap<FString, FString> stringMap; TArray<FString> stringArray; FString string; bool boolean; FString outputType; FString error;
		const bool result = DoQuery(a_config, "metadata.errors", objectArray, o, floatsArray, f, intArray, i, stringMap, stringArray, string, boolean, outputType, error);
		checkf(result, TEXT("query failed"));
		checkf(outputType == "int", TEXT("Encountered unexpected output type"));
		checkf(i == 0, TEXT("Encountered unexpected value"));
	}

	void UnitTest3(UObject* a_config)
	{
		TArray<UObject*> objectArray; UObject* o; TArray<float> floatsArray; float f; TArray<int32> intArray; int32 i; TMap<FString, FString> stringMap; TArray<FString> stringArray; FString string; bool boolean; FString outputType; FString error;
		const bool result = DoQuery(a_config, "datamodel.bathymetry.display_methods.scale.min_value", objectArray, o, floatsArray, f, intArray, i, stringMap, stringArray, string, boolean, outputType, error);
		checkf(result, TEXT("query failed"));
		checkf(outputType == "float", TEXT("Encountered unexpected output type"));
		checkf(FMath::IsNearlyZero(f - 42), TEXT("Encountered unexpected value"));
	}

	void UnitTest4(UObject* a_config)
	{
		TArray<UObject*> objectArray; UObject* o; TArray<float> floatsArray; float f; TArray<int32> intArray; int32 i; TMap<FString, FString> stringMap; TArray<FString> stringArray; FString string; bool boolean; FString outputType; FString error;
		const bool result = DoQuery(a_config, "datamodel.bathymetry.display_methods[0].scale.interpolation", objectArray, o, floatsArray, f, intArray, i, stringMap, stringArray, string, boolean, outputType, error);
		checkf(result, TEXT("query failed"));
		checkf(outputType == "class FString", TEXT("Encountered unexpected output type"));
		checkf(string == "lin", TEXT("Encountered unexpected value"));
	}

	/*void UnitTest5(UObject* a_config)
	{
		TArray<UObject*> objectArray; UObject* o; TArray<float> floatsArray; float f; TArray<int32> intArray; int32 i; TMap<FString, FString> stringMap; TArray<FString> stringArray; FString string; bool boolean; FString outputType; FString error;
		const bool result = DoQuery(a_config, "datamodel.raster_layers[name=Sediments].display_methods[method=TypeMap].mapping", objectArray, o, floatsArray, f, intArray, i, stringMap, stringArray, string, boolean, outputType, error);
		checkf(result, TEXT("query failed"));
		checkf(outputType == FormatClassNameFromUObject_Config(TEXT("class TArray<%s*>"), FDataSourceTypeMapping::StaticClass()), TEXT("Encountered unexpected output type"));
		checkf(objectArray.Num() == 4, TEXT("Encountered unexpected array count"));
	}*/

	void UnitTest6(UObject* a_config)
	{
		TArray<UObject*> objectArray; UObject* o; TArray<float> floatsArray; float f; TArray<int32> intArray; int32 i; TMap<FString, FString> stringMap; TArray<FString> stringArray; FString string; bool boolean; FString outputType; FString error;
		const bool result = DoQuery(a_config, "datamodel.vector_layers[2].display_methods[1].colour", objectArray, o, floatsArray, f, intArray, i, stringMap, stringArray, string, boolean, outputType, error);
		checkf(result, TEXT("query failed"));
		checkf(outputType == "class FString", TEXT("Encountered unexpected output type"));
		checkf(string == "#ebff0f", TEXT("Encountered unexpected value"));
	}

	void UnitTest7(UObject* a_config)
	{
		TArray<UObject*> objectArray; UObject* o; TArray<float> floatsArray; float f; TArray<int32> intArray; int32 i; TMap<FString, FString> stringMap; TArray<FString> stringArray; FString string; bool boolean; FString outputType; FString error;
		const bool result = DoQuery(a_config, "datamodel.vector_layers[name=Energy cables].display_methods[method=LineColour].method", objectArray, o, floatsArray, f, intArray, i, stringMap, stringArray, string, boolean, outputType, error);
		checkf(result, TEXT("query failed"));
		checkf(outputType == "class FString", TEXT("Encountered unexpected output type"));
		checkf(string == "LineColour", TEXT("Encountered unexpected value"));
	}

	void UnitTest8(UObject* a_config)
	{
		TArray<UObject*> objectArray; UObject* o; TArray<float> floatsArray; float f; TArray<int32> intArray; int32 i; TMap<FString, FString> stringMap; TArray<FString> stringArray; FString string; bool boolean; FString outputType; FString error;
		const bool result = DoQuery(a_config, "datamodel.vector_layers[name=National shipping lanes].labelinfo.content", objectArray, o, floatsArray, f, intArray, i, stringMap, stringArray, string, boolean, outputType, error);
		checkf(result, TEXT("query failed"));
		checkf(outputType == FormatClassNameFromUObject_Config(TEXT("class TArray<%s*>"), ULabelInfoConfigContent::StaticClass()), TEXT("Encountered unexpected output type"));
		checkf(objectArray.Num() == 3, TEXT("Encountered unexpected array count"));
	}

	void UnitTest9(UObject* a_config)
	{
		TArray<UObject*> objectArray; UObject* o; TArray<float> floatsArray; float f; TArray<int32> intArray; int32 i; TMap<FString, FString> stringMap; TArray<FString> stringArray; FString string; bool boolean; FString outputType; FString error;
		const bool result = DoQuery(a_config, "datamodel.vector_layers[name=National shipping lanes].labelinfo.content[1].ref_name", objectArray, o, floatsArray, f, intArray, i, stringMap, stringArray, string, boolean, outputType, error);
		checkf(result, TEXT("query failed"));
		checkf(outputType == "class FString", TEXT("Encountered unexpected output type"));
		checkf(string == "name", TEXT("Encountered unexpected value"));
	}

	void UnitTest10(UObject* a_config)
	{
		TArray<UObject*> objectArray; UObject* o; TArray<float> floatsArray; float f; TArray<int32> intArray; int32 i; TMap<FString, FString> stringMap; TArray<FString> stringArray; FString string; bool boolean; FString outputType; FString error;
		const bool result = DoQuery(a_config, "datamodel.vector_layers[name=Windfarms].data.points", objectArray, o, floatsArray, f, intArray, i, stringMap, stringArray, string, boolean, outputType, error);
		checkf(result, TEXT("query failed"));
		checkf(outputType == FormatClassNameFromUObject_Config(TEXT("class TArray<%s*>"), UFloatArrayWrapper::StaticClass()), TEXT("Encountered unexpected output type"));
		checkf(objectArray.Num() == 9, TEXT("Encountered unexpected array count"));
	}

	void UnitTest11(UObject* a_config)
	{
		TArray<UObject*> objectArray; UObject* o; TArray<float> floatsArray; float f; TArray<int32> intArray; int32 i; TMap<FString, FString> stringMap; TArray<FString> stringArray; FString string; bool boolean; FString outputType; FString error;
		const bool result = DoQuery(a_config, "datamodel.vector_layers[name=Windfarms].data[1].gaps", objectArray, o, floatsArray, f, intArray, i, stringMap, stringArray, string, boolean, outputType, error);
		checkf(result, TEXT("query failed"));
		checkf(outputType == FormatClassNameFromUObject_Config(TEXT("class TArray<%s*>"), UFloatArrayContainerWrapper::StaticClass()), TEXT("Encountered unexpected output type"));
		checkf(objectArray.Num() == 0, TEXT("Encountered unexpected array count"));
	}

	void UnitTest12(UObject* a_config)
	{
		TArray<UObject*> objectArray; UObject* o; TArray<float> floatsArray; float f; TArray<int32> intArray; int32 i = -1; TMap<FString, FString> stringMap; TArray<FString> stringArray; FString string; bool boolean; FString outputType; FString error;
		const bool result = DoQuery(a_config, "datamodel.vector_layers[name=Converter stations].data[0].types", objectArray, o, floatsArray, f, intArray, i, stringMap, stringArray, string, boolean, outputType, error);
		checkf(result, TEXT("query failed"));
		checkf(outputType == "class TArray<int32>", TEXT("Encountered unexpected output type"));
		checkf(intArray[0] == 1, TEXT("Encountered unexpected value"));
	}

	void UnitTest13(UObject* a_config)
	{
		TArray<UObject*> objectArray; UObject* o; TArray<float> floatsArray; float f; TArray<int32> intArray; int32 i; TMap<FString, FString> stringMap; TArray<FString> stringArray; FString string; bool boolean; FString outputType; FString error;
		const bool result = DoQuery(a_config, "datamodel.vector_layers[name=Windfarms].data[1].metadata.turbines", objectArray, o, floatsArray, f, intArray, i, stringMap, stringArray, string, boolean, outputType, error);
		checkf(result, TEXT("query failed"));
		checkf(outputType == "class FString", TEXT("Encountered unexpected output type"));
		checkf(string == FString("80"), TEXT("Encountered unexpected value"));
	}

	void UnitTest14(UObject* a_config)
	{
		TArray<UObject*> objectArray; UObject* o; TArray<float> floatsArray; float f; TArray<int32> intArray; int32 i; TMap<FString, FString> stringMap; TArray<FString> stringArray; FString string; bool boolean; FString outputType; FString error;
		const bool result = DoQuery(a_config, "datamodel.vector_layers[name=Windfarms].display_methods[method=AreaModelScatter].stages", objectArray, o, floatsArray, f, intArray, i, stringMap, stringArray, string, boolean, outputType, error);
		checkf(result, TEXT("query failed"));
		checkf(outputType == FormatClassNameFromUObject_Config(TEXT("class TArray<%s*>"), UStringMap::StaticClass()), TEXT("Encountered unexpected output type"));
		checkf(objectArray.Num() == 4, TEXT("Encountered unexpected array count"));
	}

	void UnitTest15(UObject* a_config)
	{
		TArray<UObject*> objectArray; UObject* o; TArray<float> floatsArray; float f; TArray<int32> intArray; int32 i; TMap<FString, FString> stringMap; TArray<FString> stringArray; FString string; bool boolean; FString outputType; FString error;
		const bool result = DoQuery(a_config, "datamodel.vector_layers[name=Windfarms].display_methods[method=AreaModelScatter].stage_distribution[2]", objectArray, o, floatsArray, f, intArray, i, stringMap, stringArray, string, boolean, outputType, error);
		checkf(result, TEXT("query failed"));
		checkf(outputType == "class TArray<float>", TEXT("Encountered unexpected output type"));
		checkf(FMath::IsNearlyZero(floatsArray[2] - 30), TEXT("Encountered unexpected value"));
	}

	void UnitTest16(UObject* a_config)
	{
		TArray<UObject*> objectArray; UObject* o; TArray<float> floatsArray; float f; TArray<int32> intArray; int32 i; TMap<FString, FString> stringMap; TArray<FString> stringArray; FString string; bool boolean; FString outputType; FString error;
		const bool result = DoQuery(a_config, "datamodel.vector_layers[name=Windfarms].display_methods[method=AreaModelScatter].stages[name=Idle]", objectArray, o, floatsArray, f, intArray, i, stringMap, stringArray, string, boolean, outputType, error);
		checkf(result, TEXT("query failed"));
		checkf(outputType == "class TMap<FString, FString>", TEXT("Encountered unexpected output type"));
		checkf(stringMap.Num() == 2, TEXT("Encountered unexpected array count"));
		checkf(stringMap["name"] == "Idle", TEXT("Encountered unexpected value"));
	}

	void UnitTest17(UObject* a_config)
	{
		TArray<UObject*> objectArray; UObject* o; TArray<float> floatsArray; float f; TArray<int32> intArray; int32 i; TMap<FString, FString> stringMap; TArray<FString> stringArray; FString string; bool boolean = false; FString outputType; FString error;
		const bool result = DoQuery(a_config, "datamodel.raster_layers[name=Sediments].display_methods[method=TypeMap].seabottom", objectArray, o, floatsArray, f, intArray, i, stringMap, stringArray, string, boolean, outputType, error);
		checkf(result, TEXT("query failed"));
		checkf(outputType == "bool", TEXT("Encountered unexpected output type"));
		checkf(boolean == true, TEXT("Encountered unexpected value"));
	}

	//void UnitTest18(UObject* a_config)
	//{
	//	TArray<UObject*> objectArray; UObject* o; TArray<float> floatsArray; float f; TArray<int32> intArray; int32 i; TMap<FString, FString> stringMap; TArray<FString> stringArray; FString string; bool boolean; FString outputType; FString error;
	//	const bool result = DoQuery(a_config, "datamodel.raster_layers[name=Sediments].display_methods[method=TypeMap].mapping[type=3]", objectArray, o, floatsArray, f, intArray, i, stringMap, stringArray, string, boolean, outputType, error);
	//	checkf(result, TEXT("query failed"));
	//	checkf(outputType == FormatClassNameFromUObject_Config(TEXT("class %s*"), FDataSourceTypeMapping::StaticClass()), TEXT("Encountered unexpected output type"));
	//	checkf(Cast<FDataSourceTypeMapping>(o)->max == 57, TEXT("Encountered unexpected value"));
	//}

	void UnitTest19(UObject* a_config)
	{
		TArray<UObject*> objectArray; UObject* o; TArray<float> floatsArray; float f; TArray<int32> intArray; int32 i; TMap<FString, FString> stringMap; TArray<FString> stringArray; FString string; bool boolean; FString outputType; FString error;
		const bool result = DoQuery(a_config, "datamodel.raster_layers[name=Sediments].types", objectArray, o, floatsArray, f, intArray, i, stringMap, stringArray, string, boolean, outputType, error);
		checkf(result, TEXT("query failed"));
		checkf(outputType == FormatClassNameFromUObject_Config(TEXT("class TArray<%s*>"), UStringMap::StaticClass()), TEXT("Encountered unexpected output type"));
		checkf(objectArray.Num() == 4, TEXT("Encountered unexpected array count"));
	}

	void UnitTest20(UObject* a_config)
	{
		TArray<UObject*> objectArray; UObject* o; TArray<float> floatsArray; float f; TArray<int32> intArray; int32 i; TMap<FString, FString> stringMap; TArray<FString> stringArray; FString string; bool boolean; FString outputType; FString error;
		const bool result = DoQuery(a_config, "datamodel.raster_layers[name=Sediments].types[name=Clay].material", objectArray, o, floatsArray, f, intArray, i, stringMap, stringArray, string, boolean, outputType, error);
		checkf(result, TEXT("query failed"));
		checkf(outputType == "class FString", TEXT("Encountered unexpected output type"));
		checkf(string == "Sediment2", TEXT("Encountered unexpected value"));
	}

	void UnitTest21(UObject* a_config)
	{
		// split test
		TArray<UObject*> objectArray; UObject* o; TArray<float> floatsArray; float f; TArray<int32> intArray; int32 i; TMap<FString, FString> stringMap; TArray<FString> stringArray; FString string; bool boolean; FString outputType; FString error;
		bool result = DoQuery(a_config, "datamodel.raster_layers[name=Sediments]", objectArray, o, floatsArray, f, intArray, i, stringMap, stringArray, string, boolean, outputType, error);
		checkf(result, TEXT("query failed"));
		checkf(outputType == "class URasterLayer*", TEXT("Encountered unexpected output type"));
		result = DoQuery(o, "types[name=Clay].material", objectArray, o, floatsArray, f, intArray, i, stringMap, stringArray, string, boolean, outputType, error);
		checkf(result, TEXT("query failed"));
		checkf(outputType == "class FString", TEXT("Encountered unexpected output type"));
		checkf(string == "Sediment2", TEXT("Encountered unexpected value"));
	}

	//void UnitTest22(UObject* a_config)
	//{
	//	// predicate regexp test
	//	TArray<UObject*> objectArray; UObject* o; TArray<float> floatsArray; float f; TArray<int32> intArray; int32 i; TMap<FString, FString> stringMap; TArray<FString> stringArray; FString string; bool boolean; FString outputType; FString error;
	//	const bool result = DoQuery(a_config, "datamodel.vector_layers[name=Windfarms].display_methods[method=Area.+]", objectArray, o, floatsArray, f, intArray, i, stringMap, stringArray, string, boolean, outputType, error);
	//	checkf(result, TEXT("query failed"));
	//	checkf(outputType == FormatClassNameFromUObject_Config(TEXT("class TArray<%s*>"), UADisplayMethod::StaticClass()), TEXT("Encountered unexpected output type"));
	//	checkf(objectArray.Num() == 2, TEXT("Encountered unexpected array count"));
	//}

	void UnitTest23(UObject* a_config)
	{
		// predicate hierarchy and regexp test
		TArray<UObject*> objectArray; UObject* o; TArray<float> floatsArray; float f; TArray<int32> intArray; int32 i; TMap<FString, FString> stringMap; TArray<FString> stringArray; FString string; bool boolean; FString outputType; FString error;
		bool result = DoQuery(a_config, "datamodel.vector_layers[display_methods.method=Line.+]", objectArray, o, floatsArray, f, intArray, i, stringMap, stringArray, string, boolean, outputType, error);
		checkf(result, TEXT("query failed"));
		checkf(outputType == FormatClassNameFromUObject_Config(TEXT("class TArray<%s*>"), UVectorLayer::StaticClass()), TEXT("Encountered unexpected output type"));
		checkf(objectArray.Num() == 2, TEXT("Encountered unexpected array count"));

		result = DoQuery(a_config, "datamodel.vector_layers[display_methods.method=Area.+]", objectArray, o, floatsArray, f, intArray, i, stringMap, stringArray, string, boolean, outputType, error);
		checkf(result, TEXT("query failed"));
		checkf(outputType == FormatClassNameFromUObject_Config(TEXT("class %s*"), UVectorLayer::StaticClass()), TEXT("Encountered unexpected output type"));
	}

	void UnitTest24(UObject* a_config)
	{
		// predicate hierarchy, and continuing json hierarchy path test
		TArray<UObject*> objectArray; UObject* o; TArray<float> floatsArray; float f; TArray<int32> intArray; int32 i; TMap<FString, FString> stringMap; TArray<FString> stringArray; FString string; bool boolean; FString outputType; FString error;
		DoQuery(a_config, "datamodel.vector_layers[display_methods.method=PointModel].types", objectArray, o, floatsArray, f, intArray, i, stringMap, stringArray, string, boolean, outputType, error);
		checkf(outputType == FormatClassNameFromUObject_Config(TEXT("class TArray<%s*>"), UStringMap::StaticClass()), TEXT("Encountered unexpected output type"));
		checkf(objectArray.Num() == 2, TEXT("Encountered unexpected array count"));
	}

	//void UnitTests(UObject* a_config)
	//{
	//	UnitTest1(a_config);
	//	UnitTest2(a_config);
	//	UnitTest3(a_config);
	//	UnitTest4(a_config);
	//	UnitTest5(a_config);
	//	UnitTest6(a_config);
	//	UnitTest7(a_config);
	//	UnitTest8(a_config);
	//	UnitTest9(a_config);
	//	UnitTest10(a_config);
	//	UnitTest11(a_config);
	//	UnitTest12(a_config);
	//	UnitTest13(a_config);
	//	UnitTest14(a_config);
	//	UnitTest15(a_config);
	//	UnitTest16(a_config);
	//	UnitTest17(a_config);
	//	UnitTest18(a_config);
	//	UnitTest19(a_config);
	//	UnitTest20(a_config);
	//	UnitTest21(a_config);
	//	UnitTest22(a_config);
	//	UnitTest23(a_config);
	//	UnitTest24(a_config);
	//}
}

bool UConfigBluePrintFunctions::Query(UObject* a_config, const FString& a_query, TArray<UObject*>& a_objectArray, UObject*& a_object, TArray<float>& a_floatsArray, float& a_float, TArray<int32>& a_intArray, int32& a_int, TMap<FString, FString>& a_stringMap, TArray<FString>& a_stringArray, FString& a_string, bool& a_boolean, FString& a_outputType, FString& a_error)
{
	__debugbreak();
	return false;

	/*if (a_config == nullptr) {
		a_error = "Config has not been loaded.";
		UE_LOG(LogTemp, Error, TEXT("Config has not been loaded."));
		return false;
	}
	if (a_config->IsA(UConfig::StaticClass()) &&
		Cast<UConfig>(a_config)->GetJsonFilename() == "UnitTestsInput.json") {
		UnitTests(a_config);
	}
	return DoQuery(a_config, a_query, a_objectArray, a_object, a_floatsArray, a_float, a_intArray, a_int, a_stringMap, a_stringArray, a_string, a_boolean, a_outputType, a_error);*/
}
