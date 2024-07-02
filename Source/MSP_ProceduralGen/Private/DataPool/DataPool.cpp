#include "DataPool/DataPool.h"

#include "Config/Config.h"
#include "Config/MSPConfigFilePathResolver.h"
#include "Config/Helpers/MSPConfigFileLoadHelper.h"
#include "DataPool/DataPoolDataSourceLoadedType.h"
#include "DataPool/DataPoolDataTypes.h"
#include "DataPool/DataPoolRaster.h"
#include "DataPool/Vector/DataPoolLines.h"
#include "DataPool/Vector/DataPoolPoints.h"
#include "DataPool/Vector/DataPoolPolygons.h"
#include "MSP_ProceduralGen/MSP_ProceduralGen.h"
#include "DataPool/DataPoolDataSourceNames.h"
#include "DataPool/DataPoolDataSourceTypeCollection.h"

namespace
{
	struct RasterDataSourceLocalConfig
	{
		FString m_data;
		const FRasterValueScalingData m_rasterValueScalingData;
	};
}

void UDataPool::Setup(const FMSPConfigFilePathResolver& a_pathResolver, const UConfig* a_config)
{
	m_simulatedBounds = FBox2D(a_config->datamodel->GetCoordinate0AsWorldVector2D(), a_config->datamodel->GetCoordinate1AsWorldVector2D());
	SetupDataSourcesWithDefaults();
	SetupDataSourcesFromJson(a_pathResolver, a_config);
	SetupDataSourceFromLocalFile(a_pathResolver, a_config);

	ProcessChangedEvents();

	m_dataPoolSetupFinishedSubscriptions.Broadcast(this);
	m_dataPoolSetupFinishedSubscriptions.Clear();
	m_setupFinished = true;
}

void UDataPool::RegisterDataPoolSetupFinishedListener(const FOnDataPoolSetupFinishedMulticastDelegate::FDelegate& a_listener)
{
	if (m_dataPoolSetupFinishedSubscriptions.Contains(a_listener))
	{
		return; // already added
	}
	m_dataPoolSetupFinishedSubscriptions.Add(a_listener);
	if (m_setupFinished)
	{
		m_dataPoolSetupFinishedSubscriptions.Broadcast(this);
		m_dataPoolSetupFinishedSubscriptions.Clear();
	}
}

void UDataPool::UnregisterDataPoolSetupFinishedListener(const FOnDataPoolSetupFinishedMulticastDelegate::FDelegate& a_listener)
{
	m_dataPoolSetupFinishedSubscriptions.Remove(a_listener);
}

bool UDataPool::TryRegisterDataUpdatedListener(const FName& a_dataSourceName, const FOnDataSourceDataUpdatedMulticastDelegate::FDelegate& a_listener)
{
	if (!m_dataSources.Contains(a_dataSourceName))
	{
		return false;
	}
	FOnDataSourceDataUpdatedMulticastDelegate& delegate = m_dataUpdatedSubscriptionsMap.FindOrAdd(a_dataSourceName);
	if (delegate.Contains(a_listener))
	{
		return true; // already added.
	}
	delegate.Add(a_listener);
	return true;
}

bool UDataPool::TryUnRegisterDataUpdatedListener(const FName& a_dataSourceName, const FOnDataSourceDataUpdatedMulticastDelegate::FDelegate& a_listener)
{
	if (!m_dataSources.Contains(a_dataSourceName))
	{
		return false;
	}

	FOnDataSourceDataUpdatedMulticastDelegate* delegate = m_dataUpdatedSubscriptionsMap.Find(a_dataSourceName);
	if (!delegate)
	{
		return true;
	}
	delegate->Remove(a_listener);
	return true;
}

#pragma region CreateDataSource

bool UDataPool::CreateDataSource(const FName& a_dataSourceName, const EDataPoolDataType a_dataType, const EDataPoolDataUnitType a_dataUnitType)
{
	switch (a_dataType)
	{
	case EDataPoolDataType::Float:
	{
		return CreateDataSource(a_dataSourceName, 0.0f, a_dataUnitType);
	}
	case EDataPoolDataType::Raster:
	{
		UDataPoolRaster* raster = UDataPoolRaster::CreateWithFormatAndBounds(this, a_dataSourceName, 1024, FRasterPixelChannelFormat::Gray8, m_simulatedBounds);
		raster->MakeImmutable();
		return CreateDataSource(a_dataSourceName, raster, a_dataUnitType);
	}
	case EDataPoolDataType::String:
	{
		return CreateDataSource(a_dataSourceName, FString(), a_dataUnitType);
	}
	case EDataPoolDataType::Lines:
	{
		return CreateDataSource(a_dataSourceName, FDataPoolLines(), a_dataUnitType);
	}
	case EDataPoolDataType::Points:
	{
		return CreateDataSource(a_dataSourceName, FDataPoolPoints(), a_dataUnitType);
	}
	case EDataPoolDataType::Polygons:
	{
		return CreateDataSource(a_dataSourceName, FDataPoolPolygons(), a_dataUnitType);
	}
	}

	return true;
}

bool UDataPool::CreateDataSource(const FName& a_dataSourceName, float a_data, const EDataPoolDataUnitType a_dataUnitType)
{
	if (m_dataSources.Contains(a_dataSourceName)) // already exists
	{
		if (m_dataSources[a_dataSourceName].m_dataType != EDataPoolDataType::Float)
		{
			// incompatible data type
			UE_LOG(LogMspProceduralGen, Error,
				TEXT(
					"Attempt to create a data source with name '%s' and type 'float' failed. A data source with that name already exists, but with a different type."
				), *a_dataSourceName.ToString());
			return false;
		}
		return true;
	}

	m_dataSources.Add(a_dataSourceName, FDataPoolDataSource(EDataPoolDataType::Float, a_dataUnitType, NewObject<UDataPoolDataSourceTypeCollection>()));
	m_floatContainer.Emplace(a_dataSourceName, a_data);
	return true;
}

bool UDataPool::CreateDataSource(const FName& a_dataSourceName, UDataPoolRaster* a_data, const EDataPoolDataUnitType a_dataUnitType)
{
	if (m_dataSources.Contains(a_dataSourceName)) // already exists
	{
		if (m_dataSources[a_dataSourceName].m_dataType != EDataPoolDataType::Raster)
		{
			// incompatible data type
			UE_LOG(LogMspProceduralGen, Error,
				TEXT(
					"Attempt to create a data source with name '%s' and type 'raster' failed. A data source with that name already exists, but with a different type."
				), *a_dataSourceName.ToString());
			return false;
		}
		return true;
	}

	m_dataSources.Add(a_dataSourceName, FDataPoolDataSource(EDataPoolDataType::Raster, a_dataUnitType, NewObject<UDataPoolDataSourceTypeCollection>()));
	m_rasterContainer.Emplace(a_dataSourceName, a_data);
	return true;
}

bool UDataPool::CreateDataSource(const FName& a_dataSourceName, FDataPoolLines a_data, const EDataPoolDataUnitType a_dataUnitType)
{
	if (m_dataSources.Contains(a_dataSourceName)) // already exists
	{
		if (m_dataSources[a_dataSourceName].m_dataType != EDataPoolDataType::Lines)
		{
			UE_LOG(LogMspProceduralGen, Error,
				TEXT(
					"Attempt to create a data source with name '%s' and type 'lines' failed. A data source with that name already exists, but with a different type."
				), *a_dataSourceName.ToString());
			return false;
		}
		return true;
	}

	m_dataSources.Add(a_dataSourceName, FDataPoolDataSource(EDataPoolDataType::Lines, a_dataUnitType, NewObject<UDataPoolDataSourceTypeCollection>()));
	m_linesContainer.Emplace(a_dataSourceName, MoveTemp(a_data));
	return true;
}

bool UDataPool::CreateDataSource(const FName& a_dataSourceName, FDataPoolPoints a_data, const EDataPoolDataUnitType a_dataUnitType)
{
	if (m_dataSources.Contains(a_dataSourceName)) // already exists
	{
		if (m_dataSources[a_dataSourceName].m_dataType != EDataPoolDataType::Points)
		{
			UE_LOG(LogMspProceduralGen, Error,
				TEXT(
					"Attempt to create a data source with name '%s' and type 'points' failed. A data source with that name already exists, but with a different type."
				), *a_dataSourceName.ToString());
			return false;
		}

		return true;
	}

	m_dataSources.Add(a_dataSourceName, FDataPoolDataSource(EDataPoolDataType::Points, a_dataUnitType, NewObject<UDataPoolDataSourceTypeCollection>()));
	m_pointsContainer.Emplace(a_dataSourceName, MoveTemp(a_data));
	return true;
}

bool UDataPool::CreateDataSource(const FName& a_dataSourceName, FDataPoolPolygons a_data, const EDataPoolDataUnitType a_dataUnitType)
{
	if (m_dataSources.Contains(a_dataSourceName)) // already exists
	{
		if (m_dataSources[a_dataSourceName].m_dataType != EDataPoolDataType::Polygons)
		{
			UE_LOG(LogMspProceduralGen, Error,
				TEXT(
					"Attempt to create a data source with name '%s' and type 'polygons' failed. A data source with that name already exists, but with a different type."
				), *a_dataSourceName.ToString());
			return false;
		}
		return true;
	}

	m_dataSources.Add(a_dataSourceName, FDataPoolDataSource(EDataPoolDataType::Polygons, a_dataUnitType, NewObject<UDataPoolDataSourceTypeCollection>()));
	m_polygonsContainer.Add(a_dataSourceName, a_data);
	return true;
}

bool UDataPool::CreateDataSource(const FName& a_dataSourceName, const FString& a_data, const EDataPoolDataUnitType a_dataUnitType)
{
	if (m_dataSources.Contains(a_dataSourceName)) // already exists
	{
		if (m_dataSources[a_dataSourceName].m_dataType != EDataPoolDataType::String)
		{
			UE_LOG(LogMspProceduralGen, Error,
				TEXT(
					"Attempt to create a data source with name '%s' and type 'string' failed. A data source with that name already exists, but with a different type."
				), *a_dataSourceName.ToString());
			return false;
		}
		return true;
	}

	m_dataSources.Add(a_dataSourceName, FDataPoolDataSource(EDataPoolDataType::String, a_dataUnitType, NewObject<UDataPoolDataSourceTypeCollection>()));
	m_stringContainer.Emplace(a_dataSourceName, a_data);
	return true;
}

TArray<FName> UDataPool::GetDataSourceNames() const
{
	TArray<FName> result;
	m_dataSources.GetKeys(result);
	return result;
}

bool UDataPool::GetDataType(const FName& a_dataSourceName, EDataPoolDataType& a_dataType, FString* a_errorMsg) const
{
	const FDataPoolDataSource* dataSource = m_dataSources.Find(a_dataSourceName);
	if (dataSource == nullptr)
	{
		const FString errorMsg(FString::Printf(TEXT("Encountered a invalid data source '%s'"), *a_dataSourceName.ToString()));
		if (a_errorMsg)
		{
			*a_errorMsg = errorMsg;
		}
		return false;
	}
	a_dataType = dataSource->m_dataType;
	return true;
}

bool UDataPool::GetDataUnitType(const FName& a_dataSourceName, EDataPoolDataUnitType& a_dataUnitType, FString* a_errorMsg) const
{
	const FDataPoolDataSource* dataSource = m_dataSources.Find(a_dataSourceName);
	if (dataSource == nullptr)
	{
		const FString errorMsg(FString::Printf(TEXT("Encountered a invalid data source '%s'"), *a_dataSourceName.ToString()));
		if (a_errorMsg)
		{
			*a_errorMsg = errorMsg;
		}
		return false;
	}
	a_dataUnitType = dataSource->m_dataUnitType;
	return true;
}

EDataPoolDataSourceLoadedType UDataPool::GetLoadedTypeForDataSource(const FName& a_dataSourceName) const
{
	const FDataPoolDataSource* dataSource = m_dataSources.Find(a_dataSourceName);
	if (dataSource == nullptr)
	{
		return EDataPoolDataSourceLoadedType::NotLoaded;
	}
	return dataSource->GetLoadedType();
}

bool UDataPool::GetDataTypesForDataSource(const FName& a_dataSourceName, UDataPoolDataSourceTypeCollection*& a_typeCollection) const
{
	const FDataPoolDataSource* dataSource = m_dataSources.Find(a_dataSourceName);
	if (dataSource == nullptr)
	{
		return false;
	}
	a_typeCollection = dataSource->m_layerTypes;
	return true;
}

bool UDataPool::IsDataSourceValid(const FName& a_dataSourceName, FString* a_errorMsg) const
{
	const FDataPoolDataSource* dataSource = m_dataSources.Find(a_dataSourceName);
	if (dataSource == nullptr)
	{
		if (a_errorMsg)
		{
			const FString errorMsg(FString::Printf(TEXT("Encountered a invalid data source '%s'"), *a_dataSourceName.ToString()));
			*a_errorMsg = errorMsg;
		}
		return false;
	}
	return true;
}

const FBox2D& UDataPool::GetSimulationBounds() const
{
	return m_simulatedBounds;
}

#pragma endregion
#pragma region TryGetData

bool UDataPool::TryGetData(const FName& a_dataSourceName, float& a_data) const
{
	if (!m_floatContainer.Contains(a_dataSourceName))
	{
		return false;
	}

	a_data = m_floatContainer[a_dataSourceName];
	return true;
}

bool UDataPool::TryGetData(const FName& a_dataSourceName, FString& a_data) const
{
	if (!m_stringContainer.Contains(a_dataSourceName))
	{
		return false;
	}

	a_data = m_stringContainer[a_dataSourceName];
	return true;
}

bool UDataPool::TryGetData(const FName& a_dataSourceName, IDataPoolRasterReadOnly*& a_data) const
{
	if (!m_rasterContainer.Contains(a_dataSourceName))
	{
		return false;
	}

	a_data = m_rasterContainer[a_dataSourceName];
	return true;
}

bool UDataPool::TryGetData(const FName& a_dataSourceName, FDataPoolLines& a_data) const
{
	if (!m_linesContainer.Contains(a_dataSourceName))
	{
		return false;
	}

	a_data = m_linesContainer[a_dataSourceName];
	return true;
}

bool UDataPool::TryGetData(const FName& a_dataSourceName, FDataPoolPoints& a_data) const
{
	if (!m_pointsContainer.Contains(a_dataSourceName))
	{
		return false;
	}

	a_data = m_pointsContainer[a_dataSourceName];
	return true;
}

bool UDataPool::TryGetData(const FName& a_dataSourceName, FDataPoolPolygons& a_data) const
{
	if (!m_polygonsContainer.Contains(a_dataSourceName))
	{
		return false;
	}

	a_data = m_polygonsContainer[a_dataSourceName];
	return true;
}

#pragma endregion
#pragma region TrySetData

bool UDataPool::TrySetData(const FName& a_dataSourceName, const float a_data)
{
	if (!m_floatContainer.Contains(a_dataSourceName))
	{
		return false;
	}

	TFunction<void(UDataPool*)> setFunction = [dataSourceName = a_dataSourceName, a_data](UDataPool* Target)
		{
			Target->m_floatContainer[dataSourceName] = a_data;
			Target->m_dataSources[dataSourceName].HasChanged = true;
		};
	SynchronizedSetData(MoveTemp(setFunction));
	return true;
}

bool UDataPool::TrySetData(const FName& a_dataSourceName, const FString& a_data)
{
	if (!m_stringContainer.Contains(a_dataSourceName))
	{
		return false;
	}

	TFunction<void(UDataPool*)> setFunction = [dataSourceName = a_dataSourceName, a_data](UDataPool* Target)
		{
			Target->m_stringContainer[dataSourceName] = a_data;
			Target->m_dataSources[dataSourceName].HasChanged = true;
		};
	SynchronizedSetData(MoveTemp(setFunction));

	return true;
}

bool UDataPool::TrySetData(const FName& a_dataSourceName, TStrongObjectPtr<UDataPoolRaster> a_data)
{
	if (!m_rasterContainer.Contains(a_dataSourceName))
	{
		return false;
	}

	a_data->MakeImmutable();

	TFunction<void(UDataPool*)> setFunction = [dataSourceName = a_dataSourceName, data = MoveTemp(a_data)](UDataPool* Target)
		{
			Target->m_rasterContainer.Emplace(dataSourceName, data.Get());
			Target->m_dataSources[dataSourceName].HasChanged = true;
		};
	SynchronizedSetData(MoveTemp(setFunction));
	return true;
}

bool UDataPool::TrySetData(const FName& a_dataSourceName, FDataPoolLines a_data)
{
	if (!m_linesContainer.Contains(a_dataSourceName))
	{
		return false;
	}

	TFunction<void(UDataPool*)> setFunction = [dataSourceName = a_dataSourceName, data = MoveTemp(a_data)](UDataPool* Target) mutable
		{
			Target->m_linesContainer.Emplace(dataSourceName, MoveTemp(data));
			Target->m_dataSources[dataSourceName].HasChanged = true;
		};
	SynchronizedSetData(MoveTemp(setFunction));
	return true;
}

bool UDataPool::TrySetData(const FName& a_dataSourceName, FDataPoolPoints a_data)
{
	if (!m_pointsContainer.Contains(a_dataSourceName))
	{
		return false;
	}

	TFunction<void(UDataPool*)> setFunction = [dataSourceName = a_dataSourceName, data = MoveTemp(a_data)](UDataPool* Target) mutable
		{
			Target->m_pointsContainer.Emplace(dataSourceName, MoveTemp(data));
			Target->m_dataSources[dataSourceName].HasChanged = true;
		};
	SynchronizedSetData(MoveTemp(setFunction));
	return true;
}

bool UDataPool::TrySetData(const FName& a_dataSourceName, FDataPoolPolygons a_data)
{
	if (!m_polygonsContainer.Contains(a_dataSourceName))
	{
		return false;
	}

	TFunction<void(UDataPool*)> setFunction = [dataSourceName = a_dataSourceName, data = MoveTemp(a_data)](UDataPool* Target) mutable
		{
			Target->m_polygonsContainer.Emplace(dataSourceName, MoveTemp(data));
			Target->m_dataSources[dataSourceName].HasChanged = true;
		};
	SynchronizedSetData(MoveTemp(setFunction));
	return true;
}

#pragma endregion

void UDataPool::NotifyDataSourceUpdated(const FName& a_dataSourceName)
{
	if (const auto* targetMulticastDelegate = m_dataUpdatedSubscriptionsMap.Find(a_dataSourceName))
	{
		targetMulticastDelegate->Broadcast(a_dataSourceName);
	}
}

void UDataPool::SetupDataSourcesWithDefaults()
{
	// set up the default data sources
	// float
	if (CreateDataSource(DataPoolDataSourceNames::WAVE_HEIGHT, 0.5f, EDataPoolDataUnitType::Meters))
	{
		m_dataSources[DataPoolDataSourceNames::WAVE_HEIGHT].m_layerMatching
			.SetInclLayerTags({ "WaveHeight", "ValueMap" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::WAVE_PERIOD_LENGTH, 40.0f, EDataPoolDataUnitType::Meters))
	{
		m_dataSources[DataPoolDataSourceNames::WAVE_PERIOD_LENGTH].m_layerMatching
			.SetInclLayerTags({ "WavePeriod" });
	}
	CreateDataSource(DataPoolDataSourceNames::CURRENT_DIRECTION, 140.0f, EDataPoolDataUnitType::Degrees);
	if (CreateDataSource(DataPoolDataSourceNames::CURRENT_SPEED, 0.51f, EDataPoolDataUnitType::Knots))
	{
		m_dataSources[DataPoolDataSourceNames::CURRENT_SPEED].m_layerMatching
			.SetInclLayerTags({ "Currents", "ValueMap" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::WIND_DIRECTION, 178.0f, EDataPoolDataUnitType::Degrees))
	{
		m_dataSources[DataPoolDataSourceNames::WIND_DIRECTION].m_layerMatching
			.SetInclLayerTags({ "WindDirection" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::WIND_SPEED, 15.0f, EDataPoolDataUnitType::Knots))
	{
		m_dataSources[DataPoolDataSourceNames::WIND_SPEED].m_layerMatching
			.SetInclLayerTags({ "WindSpeed", "ValueMap" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::CLOUD_COVERAGE, 0.0f, EDataPoolDataUnitType::Oktas))
	{
		m_dataSources[DataPoolDataSourceNames::CLOUD_COVERAGE].m_layerMatching
			.SetInclLayerTags({ "CloudCoverage" });
	}

	// string
	CreateDataSource(DataPoolDataSourceNames::TIME_OF_DAY, FDateTime::Now().ToString(TEXT("%H:%M")), EDataPoolDataUnitType::Time);
	CreateDataSource(DataPoolDataSourceNames::DATE, FDateTime::Now().ToString(TEXT("%Y-%m-%d")), EDataPoolDataUnitType::Date);

	// raster
	if (CreateDataSource(DataPoolDataSourceNames::BATHYMETRY, EDataPoolDataType::Raster, EDataPoolDataUnitType::Meters))
	{
		m_dataSources[DataPoolDataSourceNames::BATHYMETRY].m_layerMatching
			.SetInclLayerTags({ "Bathymetry", "ValueMap" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::SEDIMENT_TYPE, EDataPoolDataType::Raster, EDataPoolDataUnitType::ID))
	{
		m_dataSources[DataPoolDataSourceNames::SEDIMENT_TYPE].m_layerMatching
			.SetInclLayerTags({ "Sediment", "TypeMap" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::NOISE, EDataPoolDataType::Raster, EDataPoolDataUnitType::Decibel))
	{
		m_dataSources[DataPoolDataSourceNames::NOISE].m_layerMatching
			.SetInclLayerTags({ "Pressure", "Noise", "ValueMap" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::TURBIDITY, EDataPoolDataType::Raster, EDataPoolDataUnitType::NTU))
	{
		m_dataSources[DataPoolDataSourceNames::TURBIDITY].m_layerMatching
			.SetInclLayerTags({ "Turbidity" });
	}
	CreateDataSource(DataPoolDataSourceNames::BOTTOM_TURBIDITY, EDataPoolDataType::Raster, EDataPoolDataUnitType::NTU);
	if (CreateDataSource(DataPoolDataSourceNames::FISH_DENSITY_COD, EDataPoolDataType::Raster, EDataPoolDataUnitType::Other))
	{
		m_dataSources[DataPoolDataSourceNames::FISH_DENSITY_COD].m_layerMatching
			.SetInclLayerTags({ "Biomass", "Fish", "Cod", "ValueMap" });
	}
	CreateDataSource(DataPoolDataSourceNames::FISH_DENSITY_WHITING, EDataPoolDataType::Raster, EDataPoolDataUnitType::Other);
	if (CreateDataSource(DataPoolDataSourceNames::FISH_DENSITY_FLATFISH, EDataPoolDataType::Raster, EDataPoolDataUnitType::Other))
	{
		m_dataSources[DataPoolDataSourceNames::FISH_DENSITY_FLATFISH].m_layerMatching
			.SetInclLayerTags({ "Biomass", "Fish", "Flatfish", "ValueMap" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::FISH_DENSITY_DEMERSAL, EDataPoolDataType::Raster, EDataPoolDataUnitType::Other))
	{
		m_dataSources[DataPoolDataSourceNames::FISH_DENSITY_DEMERSAL].m_layerMatching
			.SetInclLayerTags({ "Biomass", "Fish", "Demersal", "ValueMap" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::FISH_DENSITY_MACKEREL, EDataPoolDataType::Raster, EDataPoolDataUnitType::Other))
	{
		m_dataSources[DataPoolDataSourceNames::FISH_DENSITY_MACKEREL].m_layerMatching
			.SetInclLayerTags({ "Biomass", "Fish", "Mackerel", "ValueMap" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::FISH_DENSITY_HERRING, EDataPoolDataType::Raster, EDataPoolDataUnitType::Other))
	{
		m_dataSources[DataPoolDataSourceNames::FISH_DENSITY_HERRING].m_layerMatching
			.SetInclLayerTags({ "Biomass", "Fish", "Herring", "ValueMap" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::FISH_DENSITY_SANDEEL, EDataPoolDataType::Raster, EDataPoolDataUnitType::Other))
	{
		m_dataSources[DataPoolDataSourceNames::FISH_DENSITY_SANDEEL].m_layerMatching
			.SetInclLayerTags({ "Biomass", "Fish", "Sandeel", "ValueMap" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::MAMMALS_DENSITY_CETACEAN, EDataPoolDataType::Raster, EDataPoolDataUnitType::Other))
	{
		m_dataSources[DataPoolDataSourceNames::MAMMALS_DENSITY_CETACEAN].m_layerMatching
			.SetInclLayerTags({ "Biomass", "Mammals", "Cetacean", "ValueMap" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::MAMMALS_DENSITY_SEAL, EDataPoolDataType::Raster, EDataPoolDataUnitType::Other))
	{
		m_dataSources[DataPoolDataSourceNames::MAMMALS_DENSITY_SEAL].m_layerMatching
			.SetInclLayerTags({ "Biomass", "Mammals", "Seal", "ValueMap" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::SHIPPING_INTENSITY_CARGO, EDataPoolDataType::Raster, EDataPoolDataUnitType::Other))
	{
		m_dataSources[DataPoolDataSourceNames::SHIPPING_INTENSITY_CARGO].m_layerMatching
			.SetInclLayerTags({ "Shipping", "ValueMap", "Cargo" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::SHIPPING_INTENSITY_FERRY, EDataPoolDataType::Raster, EDataPoolDataUnitType::Other))
	{
		m_dataSources[DataPoolDataSourceNames::SHIPPING_INTENSITY_FERRY].m_layerMatching
			.SetInclLayerTags({ "Shipping", "ValueMap", "Ferry" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::SHIPPING_INTENSITY_MAINTENANCE, EDataPoolDataType::Raster, EDataPoolDataUnitType::Other))
	{
		m_dataSources[DataPoolDataSourceNames::SHIPPING_INTENSITY_MAINTENANCE].m_layerMatching
			.SetInclLayerTags({ "Shipping", "ValueMap", "Maintenance" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::SHIPPING_INTENSITY_PASSENGER, EDataPoolDataType::Raster, EDataPoolDataUnitType::Other))
	{
		m_dataSources[DataPoolDataSourceNames::SHIPPING_INTENSITY_PASSENGER].m_layerMatching
			.SetInclLayerTags({ "Shipping", "ValueMap", "Passenger" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::SHIPPING_INTENSITY_TANKER, EDataPoolDataType::Raster, EDataPoolDataUnitType::Other))
	{
		m_dataSources[DataPoolDataSourceNames::SHIPPING_INTENSITY_TANKER].m_layerMatching
			.SetInclLayerTags({ "Shipping", "ValueMap", "Tanker" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::SHIPPING_INTENSITY_TOTAL, EDataPoolDataType::Raster, EDataPoolDataUnitType::Other))
	{
		m_dataSources[DataPoolDataSourceNames::SHIPPING_INTENSITY_TOTAL].m_layerMatching
			.SetInclLayerTags({ "Shipping", "ValueMap", "Total" });
	}

	// vector
	if (CreateDataSource(DataPoolDataSourceNames::WIND_PARK, EDataPoolDataType::Polygons, EDataPoolDataUnitType::Area))
	{
		m_dataSources[DataPoolDataSourceNames::WIND_PARK].m_layerMatching
			.SetInclLayerTags({ "Electricity", "Production", "Polygon", "Wind" }).SetExclLayerTags({ "Planned" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::WIND_PARK_PROJECTS, EDataPoolDataType::Polygons, EDataPoolDataUnitType::Area))
	{
		m_dataSources[DataPoolDataSourceNames::WIND_PARK_PROJECTS].m_layerMatching
			.SetInclLayerTags({ "Electricity", "Production", "Polygon", "Wind", "Planned" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::WAVE_PARK, EDataPoolDataType::Polygons, EDataPoolDataUnitType::Area))
	{
		m_dataSources[DataPoolDataSourceNames::WAVE_PARK].m_layerMatching
			.SetInclLayerTags({ "Electricity", "Production", "Polygon", "Wave" }).SetExclLayerTags({ "Planned" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::WAVE_PARK_PROJECTS, EDataPoolDataType::Polygons, EDataPoolDataUnitType::Area))
	{
		m_dataSources[DataPoolDataSourceNames::WAVE_PARK_PROJECTS].m_layerMatching
			.SetInclLayerTags({ "Electricity", "Production", "Polygon", "Wave", "Planned" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::TIDAL_PARK, EDataPoolDataType::Polygons, EDataPoolDataUnitType::Area))
	{
		m_dataSources[DataPoolDataSourceNames::TIDAL_PARK].m_layerMatching
			.SetInclLayerTags({ "Electricity", "Production", "Polygon", "Tidal" }).SetExclLayerTags({ "Planned" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::TIDAL_PARK_PROJECTS, EDataPoolDataType::Polygons, EDataPoolDataUnitType::Area))
	{
		m_dataSources[DataPoolDataSourceNames::TIDAL_PARK_PROJECTS].m_layerMatching
			.SetInclLayerTags({ "Electricity", "Production", "Polygon", "Tidal", "Planned" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::ENERGY_CABLES, EDataPoolDataType::Lines, EDataPoolDataUnitType::Meters))
	{
		m_dataSources[DataPoolDataSourceNames::ENERGY_CABLES].m_layerMatching
			.SetInclLayerTags({ "Electricity", "CableAndPipe", "Line" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::SHIPPING_LANES_NATIONAL, EDataPoolDataType::Lines, EDataPoolDataUnitType::Meters))
	{
		m_dataSources[DataPoolDataSourceNames::SHIPPING_LANES_NATIONAL].m_layerMatching
			.SetInclLayerTags({ "Shipping", "Lane", "Line", "National" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::SHIPPING_LANES_IMO, EDataPoolDataType::Lines, EDataPoolDataUnitType::Meters))
	{
		m_dataSources[DataPoolDataSourceNames::SHIPPING_LANES_IMO].m_layerMatching
			.SetInclLayerTags({ "Shipping", "Lane", "Line", "IMO" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::PROTECTION_AREAS_MARINE, EDataPoolDataType::Polygons, EDataPoolDataUnitType::Area))
	{
		m_dataSources[DataPoolDataSourceNames::PROTECTION_AREAS_MARINE].m_layerMatching
			.SetInclLayerTags({ "Protection", "Polygon", "Marine" });
	}
	if (CreateDataSource(DataPoolDataSourceNames::PROTECTION_AREAS_NATURA2000, EDataPoolDataType::Polygons, EDataPoolDataUnitType::Area))
	{
		m_dataSources[DataPoolDataSourceNames::PROTECTION_AREAS_NATURA2000].m_layerMatching
			.SetInclLayerTags({ "Protection", "Polygon", "Natura2000" });
	}
	CreateDataSource(DataPoolDataSourceNames::AREAS_OF_INTEREST, EDataPoolDataType::Points, EDataPoolDataUnitType::Other);
	CreateDataSource(DataPoolDataSourceNames::NOISE_SOURCE, EDataPoolDataType::Points, EDataPoolDataUnitType::Other);

	//Setup noise layer min & max
	if (UDataPoolRaster** noiseLayer = m_rasterContainer.Find(DataPoolDataSourceNames::NOISE))
	{
		(*noiseLayer)->SetValueMapping({ .OutputValueRangeMin = 0.0, .OutputValueRangeMax = 140 }); //A Jet engine at takeoff (140dB) sounds like a reasonable default
	}
}

void UDataPool::SetupDataSourcesFromJson(const FMSPConfigFilePathResolver& a_pathResolver, const UConfig* a_config)
{
	for (const auto& rasterLayer : a_config->datamodel->raster_layers)
	{
		SetupLayerTypesFromConfig(*rasterLayer, EDataPoolDataType::Raster);
		const FName dataSourceName(GetTranslatedDataSourceNameFromConfig(*rasterLayer));
		if (!SetupRasterSourceFromConfig(
			a_pathResolver,
			rasterLayer->name,
			dataSourceName,
			rasterLayer->data,
			rasterLayer->GetCoordinate0AsWorldVector2D(),
			rasterLayer->GetCoordinate1AsWorldVector2D(),
			rasterLayer->scale,
			FRasterTypeMapping(*rasterLayer),
			a_config->datamodel->GetWorldOffset()
		))
		{
			continue;
		}
		m_dataSources[dataSourceName].SetLoadedType(EDataPoolDataSourceLoadedType::Json);
	}
	for (const auto& vectorLayer : a_config->datamodel->vector_layers)
	{
		SetupLayerTypesFromConfig(*vectorLayer, vectorLayer->GetDataTypeFromTags());
		SetupVectorSourceFromConfig(vectorLayer);
	}
}

void UDataPool::SetupDataSourceFromLocalFile(const FMSPConfigFilePathResolver& a_pathResolver, const UConfig* a_config)
{
	// Setup hard-coded local configs for the raster data sources, as fall-back, if it is not defined in the config file
	TMap<EDataPoolDataSource, RasterDataSourceLocalConfig> rasterDataSourceLocalConfigs;
	rasterDataSourceLocalConfigs.Emplace(
		EDataPoolDataSource::Bathymetry,
		RasterDataSourceLocalConfig{
			.m_data = FMSPConfigFileLoadHelper::BASE_PATH_FROM_CONTENT_DIR + a_config->metadata->region + "/Bathymetry_dm_uint16.tif",
			.m_rasterValueScalingData = FRasterValueScalingData{
				.min_value = 0.0f,
				.max_value = -655360.0f,
				.interpolation = ERasterValueMappingInterpolation::Lin
			}
		}
	);
	for (auto& localConfig : rasterDataSourceLocalConfigs)
	{
		const FName dataSourceName(DataPoolDataSourceNames::GetName(localConfig.Key));
		if (m_dataSources[dataSourceName].m_loadedType == EDataPoolDataSourceLoadedType::Json)
		{
			continue; // already loaded from json
		}
		const FVector2D worldOffset = a_config->datamodel->GetWorldOffset();
		if (!SetupRasterSourceFromConfig(
			a_pathResolver,
			localConfig.Value.m_data,
			dataSourceName,
			localConfig.Value.m_data,
			m_simulatedBounds.Min,
			m_simulatedBounds.Max,
			localConfig.Value.m_rasterValueScalingData,
			FRasterTypeMapping(), // we only support hard-coded configs for ValueMaps now
			worldOffset
		))
		{
			continue;
		}
		m_dataSources[dataSourceName].SetLoadedType(EDataPoolDataSourceLoadedType::LocalFile);
	}
}

void UDataPool::SetupPointsSourceFromConfig(const FName& a_dataSourceName, UVectorLayer* a_vectorLayer)
{
	FDataPoolPoints points;
	for (const auto& vectorObject : a_vectorLayer->data)
	{
		FDataPoolPoint point;
		UDataPoolDataSourceTypeCollection* layerTypes = nullptr;
		GetDataTypesForDataSource(a_dataSourceName, layerTypes);
		point.SetupFromConfig(a_vectorLayer, vectorObject, layerTypes);
		points.m_points.Add(point);
	}

	bool result = TrySetData(a_dataSourceName, MoveTemp(points));
	if (!result)
	{
		UE_LOG(LogMspProceduralGen, Error, TEXT("Failed to setup Polygons from config. Data layer: %s"), *a_dataSourceName.ToString());
	}
}

void UDataPool::SetupLinesSourceFromConfig(const FName& a_dataSourceName, UVectorLayer* a_vectorLayer)
{
	FDataPoolLines lines;
	for (const auto& vectorObject : a_vectorLayer->data)
	{
		FDataPoolLine line;
		UDataPoolDataSourceTypeCollection* layerTypes = nullptr;
		GetDataTypesForDataSource(a_dataSourceName, layerTypes);
		line.SetupFromConfig(a_vectorLayer, vectorObject, layerTypes);
		lines.m_lines.Add(line);
	}

	bool result = TrySetData(a_dataSourceName, MoveTemp(lines));
	if (!result)
	{
		UE_LOG(LogMspProceduralGen, Error, TEXT("Failed to setup Polygons from config. Data layer: %s"), *a_dataSourceName.ToString());
	}
}

void UDataPool::SetupPolygonsSourceFromConfig(const FName& a_dataSourceName, UVectorLayer* a_vectorLayer)
{
	FDataPoolPolygons polygons;
	for (const auto& vectorObject : a_vectorLayer->data)
	{
		FDataPoolPolygon polygon;
		UDataPoolDataSourceTypeCollection* layerTypes = nullptr;
		GetDataTypesForDataSource(a_dataSourceName, layerTypes);
		polygon.SetupFromConfig(a_vectorLayer, vectorObject, layerTypes);
		polygons.m_polygons.Add(polygon);
	}

	bool result = TrySetData(a_dataSourceName, MoveTemp(polygons));
	if (!result)
	{
		UE_LOG(LogMspProceduralGen, Error, TEXT("Failed to setup Polygons from config. Data layer: %s"), *a_dataSourceName.ToString());
	}
}

bool UDataPool::SetupVectorSourceFromConfig(
	UVectorLayer* a_vectorLayer
)
{
	const FName dataSourceName = GetTranslatedDataSourceNameFromConfig(*a_vectorLayer);

	switch (a_vectorLayer->GetDataTypeFromTags())
	{
	case EDataPoolDataType::Points:
	{
		SetupPointsSourceFromConfig(dataSourceName, a_vectorLayer);
		break;
	}
	case EDataPoolDataType::Lines:
	{
		SetupLinesSourceFromConfig(dataSourceName, a_vectorLayer);
		break;
	}
	default:
	case EDataPoolDataType::Polygons:
	{
		SetupPolygonsSourceFromConfig(dataSourceName, a_vectorLayer);
		break;
	}
	}

	m_dataSources[dataSourceName].SetLoadedType(EDataPoolDataSourceLoadedType::Json);
	return true;
}

bool UDataPool::SetupRasterSourceFromConfig(
	const FMSPConfigFilePathResolver& a_pathResolver,
	const FString& a_rasterLayerName,
	FName a_rasterDataSourceName,
	const FString& a_rasterLayerData,
	const FVector2D& a_rasterCoordinate0,
	const FVector2D& a_rasterCoordinate1,
	const FRasterValueScalingData& a_rasterValueScalingData,
	const FRasterTypeMapping& a_rasterTypeMapping,
	const FVector2D& a_configDataModelWorldOffset
) {
	FRasterPixelData pixelData{};
	FRasterProjection projection{};
	UDataPoolRaster::FRasterLinearValueMapping linearMapping{};
	bool fileContainsProjection{};
	const FString resolvedFilePath = a_pathResolver.ResolvePath(a_rasterLayerData);
	if (!UDataPoolRaster::LoadImageDataFromImageFile(resolvedFilePath, pixelData, projection, fileContainsProjection, linearMapping))
	{
		UE_LOG(
			LogMspProceduralGen,
			Error,
			TEXT("%s: Could not create byte array from image file: %s, skipping data pool source: %s"),
			*FString(__FUNCTION__),
			*a_rasterLayerData,
			*a_rasterDataSourceName.ToString()
		);
		return false;
	}

	if (fileContainsProjection)
	{
		projection.OriginWorldSpaceUnrealUnits -= FVector2D(a_configDataModelWorldOffset.X, -a_configDataModelWorldOffset.Y);
	}
	else
	{
		projection = FRasterProjection::CreateForBounds(a_rasterCoordinate0, a_rasterCoordinate1, pixelData);
	}

	UDataPoolRaster::FRasterLinearGroupedValueMapping linearGroupedMapping;
	if (a_rasterValueScalingData.interpolation == ERasterValueMappingInterpolation::Lin)
	{
		if (a_rasterValueScalingData.min_value != FRasterValueScalingData::UnsetValue)
		{
			linearMapping.OutputValueRangeMin = a_rasterValueScalingData.min_value;
		}
		if (a_rasterValueScalingData.max_value != FRasterValueScalingData::UnsetValue)
		{
			linearMapping.OutputValueRangeMax = a_rasterValueScalingData.max_value;
		}
		linearGroupedMapping = UDataPoolRaster::FRasterLinearGroupedValueMapping(linearMapping);
	}
	else
	{
		verify(a_rasterValueScalingData.interpolation == ERasterValueMappingInterpolation::LinGrouped);
		for (const auto [normalised_input_value, min_output_value] : a_rasterValueScalingData.groups)
		{
			linearGroupedMapping.Items.Add(
				UDataPoolRaster::FRasterLinearGroupedItem(normalised_input_value, min_output_value)
			);
		}
	}

	UDataPoolRaster* raster = NewObject<UDataPoolRaster>(this, FName(a_rasterDataSourceName));
	raster->Init(a_rasterDataSourceName, MoveTemp(pixelData), MoveTemp(projection), MoveTemp(linearGroupedMapping), a_rasterTypeMapping);

	bool result = TrySetData(a_rasterDataSourceName, TStrongObjectPtr(raster));
	if (!result)
	{
		UE_LOG(LogMspProceduralGen, Error, TEXT("%s: Failed to update raster data source for setup: %s"), *FString(__FUNCTION__), *a_rasterLayerName);
		return false;
	}

	return result;
}

void UDataPool::SetupLayerTypesFromConfig(const UALayer& a_layerFromConfig, const EDataPoolDataType a_dataType)
{
	const FName targetLayer = GetTranslatedDataSourceNameFromConfig(a_layerFromConfig);
	const FDataPoolDataSource* dataSource = m_dataSources.Find(targetLayer);
	if (dataSource == nullptr)
	{
		if (!CreateDataSource(targetLayer, a_dataType, EDataPoolDataUnitType::Other))
		{
			return;
		}
		dataSource = m_dataSources.Find(targetLayer);
	}
	dataSource->m_layerTypes->AddTypesFromConfig(a_layerFromConfig.types);
}

FName UDataPool::GetTranslatedDataSourceNameFromConfig(const UALayer& a_layerFromConfig) const
{
	for (const auto& dataSource : m_dataSources)
	{
		if (dataSource.Value.IsMatchingLayer(a_layerFromConfig))
		{
			return dataSource.Key;
		}
	}
	return *a_layerFromConfig.name;
}

void UDataPool::ProcessChangedEvents()
{
	TArray<TFunction<void(UDataPool*)>> callbacksToRunThisFrame;
	{
		FScopeLock lock(&m_dataUpdatedCallbackQueueMutex);
		callbacksToRunThisFrame = MoveTemp(m_dataUpdatedCallbackQueue);
		m_dataUpdatedCallbackQueue = {};
	}

	for (auto& callback : callbacksToRunThisFrame)
	{
		callback(this);
	}

	auto dataSourcesCopy = m_dataSources;

	for (auto& dataSource : m_dataSources)
	{
		dataSource.Value.HasChanged = false;
	}

	for (auto& dataSource : dataSourcesCopy)
	{
		if (dataSource.Value.HasChanged)
		{
			NotifyDataSourceUpdated(dataSource.Key);
		}
	}
}

void UDataPool::SynchronizedSetData(TFunction<void(UDataPool*)> SetFunction)
{
	if (IsInGameThread())
	{
		SetFunction(this);
	}
	else
	{
		FScopeLock lock(&m_dataUpdatedCallbackQueueMutex);
		m_dataUpdatedCallbackQueue.Emplace(MoveTemp(SetFunction));
	}
}
