// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerationSettings.h"

#include "UObject/UnrealType.h" 
#include "UObject\UnrealTypePrivate.h"

void UGenerationSettingsFuncLib::GetGenerationSettingsPropertyArray(TArray<FPropertyStruct>& OutProperties)
{
	const UGenerationSettings* defaultGenerationSettings = NewObject<UGenerationSettings>();

	for (TFieldIterator<FProperty> It(defaultGenerationSettings->GetClass()); It; ++It)
	{
		FProperty* Property = *It;

		// put void* value into FString
		FString ValueString = "";
		Property->ExportTextItem_InContainer(ValueString, defaultGenerationSettings, "", nullptr, 0);
		
		OutProperties.Add(
			{ 
				Property->GetNameCPP(), 
				Property->GetCPPType(), 
				ValueString
			});
	}
}

void UGenerationSettingsFuncLib::SetPropertyGenerationSettings(UGenerationSettings* Container, const FPropertyStruct& PropertyStruct, FString Value)
{
	for (TFieldIterator<FProperty> It(Container->GetClass()); It; ++It)
	{
		FProperty* Property = *It;

		if (Property->GetNameCPP() == PropertyStruct.Name)
		{
			Property->ImportText_InContainer(*Value, Container, nullptr, 0);
		}
	}
}

void UGenerationSettingsFuncLib::GetPropertyGenerationSettings(UGenerationSettings *Container, const FPropertyStruct &PropertyStruct, FString &Value)
{
	for (TFieldIterator<FProperty> It(Container->GetClass()); It; ++It)
	{
		FProperty* Property = *It;

		if (Property->GetNameCPP() == PropertyStruct.Name)
		{
			Property->ExportTextItem_InContainer(Value, Container, "", nullptr, 0);
		}
	}
}

bool UGenerationSettingsFuncLib::CallFunctionByName(UObject* ObjPtr, FName FunctionName)
{
	if (ObjPtr)
	{
		if (UFunction* Function = ObjPtr->FindFunction(FunctionName))
		{
			ObjPtr->ProcessEvent(Function, nullptr);
			return true;
		}
		return false;
	}
	return false;
}


