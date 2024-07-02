#pragma once

UENUM(BlueprintType)
enum class EDataPoolDataType : uint8
{
	Lines = 3 UMETA(DisplayName = "lines"),
	Points = 4 UMETA(DisplayName = "points"),
	Polygons = 5 UMETA(DisplayName = "polygons"),
    Float = 0 UMETA(DisplayName = "float"),
    Raster = 1 UMETA(DisplayName = "raster"),
    String = 2 UMETA(DisplayName = "string"),
};

class DataPoolDataTypes
{
public:
	inline static const FString FLOAT = "float";
	inline static const FString LINES = "lines";
	inline static const FString POINTS = "points";
	inline static const FString POLYGONS = "polygons";
	inline static const FString RASTER = "raster";
	inline static const FString STRING = "string";

	static void LoopByDelegate(const TFunction<void(const EDataPoolDataType&)>& a_delegate)
	{
		for (auto nameToEnum : DATA_TYPE_TO_ENUM) {
			a_delegate(nameToEnum.Value);
		}
	}

	static bool TryGetDataTypeFromName(const FString& a_dataTypeName, EDataPoolDataType& a_dataType)
	{
		if (!DATA_TYPE_TO_ENUM.Contains(a_dataTypeName)) return false;
		a_dataType = DATA_TYPE_TO_ENUM[a_dataTypeName];
		return true;
	}

	static const FString& GetName(const EDataPoolDataType& a_dataType)
	{
		return *DATA_TYPE_TO_ENUM.FindKey(a_dataType);
	}

	static bool ValidateName(const FString& a_dataTypeName)
    {
        return DATA_TYPE_TO_ENUM.Contains(a_dataTypeName);
    }

private:
	inline static const TMap<FString, EDataPoolDataType> DATA_TYPE_TO_ENUM = {
		{ FLOAT, EDataPoolDataType::Float },
		{ RASTER, EDataPoolDataType::Raster },
		{ STRING, EDataPoolDataType::String },
		{ LINES, EDataPoolDataType::Lines },
		{ POINTS, EDataPoolDataType::Points },
		{ POLYGONS, EDataPoolDataType::Polygons },
	};
};
