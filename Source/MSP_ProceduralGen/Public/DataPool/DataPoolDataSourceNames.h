#pragma once

UENUM(BlueprintType)
enum class EDataPoolDataSource : uint8
{
	AreasOfInterest = 20,
	Bathymetry = 9,
	BottomTurbidity = 13,
	CloudCoverage = 6,
	CurrentDirection = 2,
	CurrentSpeed = 3,
	Date = 8,
	EnergyCables = 17,
	FishDensityCod = 14,
	FishDensityDemersal = 23,
	FishDensityFlatfish = 22,
	FishDensityHerring = 25,
	FishDensityMackerel = 24,
	FishDensitySandeel = 26,
	FishDensityWhiting = 15,
	MammalsDensityCetacean = 27,
	MammalsDensitySeal = 28,
	Noise = 11,
	NoiseSource = 21,
	ProtectionAreasMarine = 19,
	ProtectionAreasNatura2000 = 36,
	SedimentType = 10,
	ShippingIntensityCargo = 39,
	ShippingIntensityFerry = 37,
	ShippingIntensityMaintenance = 41,
	ShippingIntensityPassenger = 38,
	ShippingIntensityTanker = 40,
	ShippingIntensityTotal = 29,
	ShippingLanesImo = 35,
	ShippingLanesNational = 18,
	TidalPark = 33,
	TidalParkProjects = 34,
	TimeOfDay = 7,
	Turbidity = 12,
	WaveHeight = 0,
	WavePark = 31,
	WaveParkProjects = 32,
	WavePeriodLength = 1,
	WindDirection = 4,
	WindPark = 16,
	WindParkProjects = 30,
	WindSpeed = 5
};

class DataPoolDataSourceNames
{
public:
	// float
	inline static const FName WAVE_HEIGHT = "wave_height"; // 0
	inline static const FName WAVE_PERIOD_LENGTH = "wave_period_length"; // 1
	inline static const FName CURRENT_DIRECTION = "current_direction"; // 2
	inline static const FName CURRENT_SPEED = "current_speed"; // 3
	inline static const FName WIND_DIRECTION = "wind_direction"; // 4
	inline static const FName WIND_SPEED = "wind_speed"; // 5
	inline static const FName CLOUD_COVERAGE = "cloud_coverage"; // 6
	//string
	inline static const FName TIME_OF_DAY = "time_of_day"; // 7
	inline static const FName DATE = "date"; // 8
	// raster
	inline static const FName BATHYMETRY = "bathymetry"; // 9
	inline static const FName SEDIMENT_TYPE = "sediment_type"; // 10
	inline static const FName NOISE = "noise"; // 11
	inline static const FName TURBIDITY = "turbidity"; // 12
	inline static const FName BOTTOM_TURBIDITY = "bottom_turbidity"; // 13
	inline static const FName FISH_DENSITY_COD = "fish_density_cod"; // 14
	inline static const FName FISH_DENSITY_WHITING = "fish_density_whiting"; // 15
	inline static const FName FISH_DENSITY_FLATFISH = "fish_density_flatfish"; // 22
	inline static const FName FISH_DENSITY_DEMERSAL = "fish_density_demersal"; // 23
	inline static const FName FISH_DENSITY_MACKEREL = "fish_density_mackerel"; // 24
	inline static const FName FISH_DENSITY_HERRING = "fish_density_herring"; // 25
	inline static const FName FISH_DENSITY_SANDEEL = "fish_density_sandeel"; // 26
	inline static const FName MAMMALS_DENSITY_CETACEAN = "mammals_density_cetacean"; // 27
	inline static const FName MAMMALS_DENSITY_SEAL = "mammals_density_seal"; // 28
	inline static const FName SHIPPING_INTENSITY_TOTAL = "shipping_intensity_total"; // 29
	inline static const FName SHIPPING_INTENSITY_FERRY = "shipping_intensity_ferry"; // 37
	inline static const FName SHIPPING_INTENSITY_PASSENGER = "shipping_intensity_passenger"; // 38
	inline static const FName SHIPPING_INTENSITY_CARGO = "shipping_intensity_cargo"; // 39
	inline static const FName SHIPPING_INTENSITY_TANKER = "shipping_intensity_tanker"; // 40
	inline static const FName SHIPPING_INTENSITY_MAINTENANCE = "shipping_intensity_maintenance"; // 41

	// vector
	inline static const FName WIND_PARK = "wind_park"; // 16
	inline static const FName WIND_PARK_PROJECTS = "wind_park_projects"; // 30
	inline static const FName WAVE_PARK = "wave_park"; // 31
	inline static const FName WAVE_PARK_PROJECTS = "wave_park_projects"; // 32
	inline static const FName TIDAL_PARK = "tidal_park"; // 33
	inline static const FName TIDAL_PARK_PROJECTS = "tidal_park_projects"; // 34
	inline static const FName ENERGY_CABLES = "energy_cables"; // 17
	inline static const FName SHIPPING_LANES_IMO = "shipping_lanes_imo"; // 35
	inline static const FName SHIPPING_LANES_NATIONAL = "shipping_lanes_national"; // 18
	inline static const FName PROTECTION_AREAS_MARINE = "protection_areas_marine"; // 19
	inline static const FName PROTECTION_AREAS_NATURA2000 = "protection_areas_natura2000"; // 36
	inline static const FName AREAS_OF_INTEREST = "areas_of_interest"; // 20
	inline static const FName NOISE_SOURCE = "noise_source"; // 21

	static void LoopByDelegate(const TFunction<void(const EDataPoolDataSource&)>& a_delegate)
	{
		for (auto nameToEnum : DATA_SOURCE_NAME_TO_ENUM) {
			a_delegate(nameToEnum.Value);
		}
	}

	static bool TryGetDataSourceFromName(const FName& a_dataSourceName, EDataPoolDataSource& a_dataSource)
	{
		if (!DATA_SOURCE_NAME_TO_ENUM.Contains(a_dataSourceName)) return false;
		a_dataSource = DATA_SOURCE_NAME_TO_ENUM[a_dataSourceName];
		return true;
	}

	static const FName& GetName(const EDataPoolDataSource& a_dataSource)
	{
		return *DATA_SOURCE_NAME_TO_ENUM.FindKey(a_dataSource);
	}

	static bool ValidateName(const FName& a_dataSourceName)
    {
        return DATA_SOURCE_NAME_TO_ENUM.Contains(a_dataSourceName);
    }

private:
	inline static const TMap<FName, EDataPoolDataSource> DATA_SOURCE_NAME_TO_ENUM = {
		{ WAVE_HEIGHT, EDataPoolDataSource::WaveHeight },
		{ WAVE_PERIOD_LENGTH, EDataPoolDataSource::WavePeriodLength },
		{ CURRENT_DIRECTION, EDataPoolDataSource::CurrentDirection },
		{ CURRENT_SPEED, EDataPoolDataSource::CurrentSpeed },
		{ WIND_DIRECTION, EDataPoolDataSource::WindDirection },
		{ WIND_SPEED, EDataPoolDataSource::WindSpeed },
		{ CLOUD_COVERAGE, EDataPoolDataSource::CloudCoverage },
		{ TIME_OF_DAY, EDataPoolDataSource::TimeOfDay },
		{ DATE, EDataPoolDataSource::Date },
		{ BATHYMETRY, EDataPoolDataSource::Bathymetry },
		{ SEDIMENT_TYPE, EDataPoolDataSource::SedimentType },
		{ NOISE, EDataPoolDataSource::Noise },
		{ TURBIDITY, EDataPoolDataSource::Turbidity },
		{ BOTTOM_TURBIDITY, EDataPoolDataSource::BottomTurbidity },
		{ FISH_DENSITY_COD, EDataPoolDataSource::FishDensityCod },
		{ FISH_DENSITY_WHITING, EDataPoolDataSource::FishDensityWhiting },
		{ FISH_DENSITY_FLATFISH, EDataPoolDataSource::FishDensityFlatfish },
		{ FISH_DENSITY_DEMERSAL, EDataPoolDataSource::FishDensityDemersal },
		{ FISH_DENSITY_MACKEREL, EDataPoolDataSource::FishDensityMackerel },
		{ FISH_DENSITY_HERRING, EDataPoolDataSource::FishDensityHerring },
		{ FISH_DENSITY_SANDEEL, EDataPoolDataSource::FishDensitySandeel },
		{ MAMMALS_DENSITY_CETACEAN, EDataPoolDataSource::MammalsDensityCetacean },
		{ MAMMALS_DENSITY_SEAL, EDataPoolDataSource::MammalsDensitySeal },
		{SHIPPING_INTENSITY_FERRY, EDataPoolDataSource::ShippingIntensityFerry },
		{ SHIPPING_INTENSITY_PASSENGER, EDataPoolDataSource::ShippingIntensityPassenger },
		{ SHIPPING_INTENSITY_CARGO, EDataPoolDataSource::ShippingIntensityCargo },
		{ SHIPPING_INTENSITY_TANKER, EDataPoolDataSource::ShippingIntensityTanker },
		{ SHIPPING_INTENSITY_MAINTENANCE, EDataPoolDataSource::ShippingIntensityMaintenance },
		{ SHIPPING_INTENSITY_TOTAL, EDataPoolDataSource::ShippingIntensityTotal },
		{ WIND_PARK, EDataPoolDataSource::WindPark },
		{ WIND_PARK_PROJECTS, EDataPoolDataSource::WindParkProjects },
		{ WAVE_PARK, EDataPoolDataSource::WavePark },
		{ WAVE_PARK_PROJECTS, EDataPoolDataSource::WaveParkProjects },
		{ TIDAL_PARK, EDataPoolDataSource::TidalPark },
		{ TIDAL_PARK_PROJECTS, EDataPoolDataSource::TidalParkProjects },
		{ ENERGY_CABLES, EDataPoolDataSource::EnergyCables },
		{ SHIPPING_LANES_IMO, EDataPoolDataSource::ShippingLanesImo },
		{ SHIPPING_LANES_NATIONAL, EDataPoolDataSource::ShippingLanesNational },
		{ PROTECTION_AREAS_MARINE, EDataPoolDataSource::ProtectionAreasMarine },
		{ PROTECTION_AREAS_NATURA2000, EDataPoolDataSource::ProtectionAreasNatura2000 },
		{ AREAS_OF_INTEREST, EDataPoolDataSource::AreasOfInterest },
		{ NOISE_SOURCE, EDataPoolDataSource::NoiseSource }
	};
};
