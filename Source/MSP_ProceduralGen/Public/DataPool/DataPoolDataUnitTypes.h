#pragma once

UENUM(BlueprintType)
enum class EDataPoolDataUnitType : uint8
{
	Area = 0 UMETA(DisplayName = "area (m2)"),
	Date = 1 UMETA(DisplayName = "date (yyyy-MM-dd)"),
	Decibel = 2 UMETA(DisplayName = "decibel (dB)"),
	Degrees = 3 UMETA(DisplayName = "degrees (0-360)"),
	ID = 4 UMETA(DisplayName = "ID"),
	Knots = 5 UMETA(DisplayName = "knots"),
	Meters = 6 UMETA(DisplayName = "meters"),
	NTU = 7 UMETA(DisplayName = "Nephelometric Turbidity Units (0-128)"),
	Oktas = 8 UMETA(DisplayName = "oktas"),
	Time = 9 UMETA(DisplayName = "time (hh:mm)"),

	Other = 10 UMETA(DisplayName = "other"),
};

class DataPoolDataUnitTypes
{
public:
	inline static const FString AREA = "area";
	inline static const FString DATE = "date";
	inline static const FString DECIBEL = "decibel";
	inline static const FString DEGREES = "degrees";
	inline static const FString ID = "ID";
	inline static const FString KNOTS = "knots";
	inline static const FString METERS = "meters";
	inline static const FString NTU = "NTU";
	inline static const FString OKTAS = "oktas";
	inline static const FString TIME = "time";

	inline static const FString OTHER = "other";

	static void LoopByDelegate(const TFunction<void(const EDataPoolDataUnitType&)>& a_delegate)
	{
		for (auto nameToEnum : DATA_UNIT_TYPE_TO_ENUM) {
			a_delegate(nameToEnum.Value);
		}
	}

	static bool TryGetDataUnitTypeFromName(const FString& a_dataTypeUnitName, EDataPoolDataUnitType& a_dataUnitType)
	{
		if (!DATA_UNIT_TYPE_TO_ENUM.Contains(a_dataTypeUnitName)) return false;
		a_dataUnitType = DATA_UNIT_TYPE_TO_ENUM[a_dataTypeUnitName];
		return true;
	}

	static const FString& GetName(const EDataPoolDataUnitType& a_dataUnitType)
	{
		return *DATA_UNIT_TYPE_TO_ENUM.FindKey(a_dataUnitType);
	}

	static bool ValidateName(const FString& a_dataTypeUnitName)
    {
        return DATA_UNIT_TYPE_TO_ENUM.Contains(a_dataTypeUnitName);
    }

private:
	inline static const TMap<FString, EDataPoolDataUnitType> DATA_UNIT_TYPE_TO_ENUM = {
		{ AREA, EDataPoolDataUnitType::Area },
		{ DATE, EDataPoolDataUnitType::Date },
		{ DECIBEL, EDataPoolDataUnitType::Decibel },
		{ DEGREES, EDataPoolDataUnitType::Degrees },
		{ ID, EDataPoolDataUnitType::ID },
		{ KNOTS, EDataPoolDataUnitType::Knots },
		{ METERS, EDataPoolDataUnitType::Meters },
		{ NTU, EDataPoolDataUnitType::NTU },
		{ OKTAS, EDataPoolDataUnitType::Oktas },
		{ TIME, EDataPoolDataUnitType::Time },
		{ OTHER, EDataPoolDataUnitType::Other },
	};
};
