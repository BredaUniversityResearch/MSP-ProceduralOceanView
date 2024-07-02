#pragma once

#include "JsonUtilities.h"
#include "Config/BluePrint/ConfigBluePrintFunctions.h"

template<typename T>
class JsonFileLoadHelper
{
public:
	static const FString BASE_PATH_FROM_CONTENT_DIR;

	virtual ~JsonFileLoadHelper()
	{
		m_loadedData = nullptr;
	}

	const FString& GetJsonFilePath()
	{
		return m_jsonFilePath;
	}

	T* Load(const FString& a_jsonFilePath)
	{
		const FString filePath = FPaths::ProjectContentDir() + BASE_PATH_FROM_CONTENT_DIR + a_jsonFilePath;
		m_jsonFilePath = a_jsonFilePath;
		if (FPaths::DirectoryExists(filePath)) { // ah, so you gave me a directory? I'll just append the default config file name to it
			m_jsonFilePath += "/" + UConfigBluePrintFunctions::GetJsonConfigFilename();
		}
		m_loadedData = nullptr;
		if (const auto& jsonObject = LoadJsonFileToObject(m_jsonFilePath); jsonObject != nullptr) {
			m_loadedData = ConvertJsonObjectToClass(jsonObject);
		}

		return m_loadedData;
	}

private:
	T* ConvertJsonObjectToClass(const TSharedPtr<FJsonObject>& a_jsonObject)
	{
		T* result = NewObject<T>(); // this means T needs to be a UObject*
		if (false == FJsonObjectConverter::JsonObjectToUStruct(a_jsonObject.ToSharedRef(), T::StaticClass(), result, 0, CPF_SkipSerialization)) {
			return nullptr;
		}
		OnPostConvertJsonObjectToClass(a_jsonObject, result);
		return result;
	}

	TSharedPtr<FJsonObject> LoadJsonFileToObject(const FString& a_jsonFilePath) const
	{
		const FString filePath = FPaths::ProjectContentDir() + BASE_PATH_FROM_CONTENT_DIR + a_jsonFilePath;
		TSharedPtr<FJsonObject> jsonObject;
		FString jsonString;
		if (!FFileHelper::LoadFileToString(jsonString, *filePath))
		{
			// Failed to load the JSON file
			return jsonObject;
		}
		const TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(jsonString);
		FJsonSerializer::Deserialize(reader, jsonObject);
		return jsonObject;
	}

protected:
	virtual void OnPostConvertJsonObjectToClass(TSharedPtr<FJsonObject> a_jsonObject, T* a_result) {};

private:
	FString m_jsonFilePath;
	T* m_loadedData = nullptr;
};

template<typename T> const FString JsonFileLoadHelper<T>::BASE_PATH_FROM_CONTENT_DIR = UConfigBluePrintFunctions::GetJsonBasePathFromDir();
