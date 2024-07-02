#pragma once

#include "Config/RasterLayer.h"
#include "DataPool/DataPoolDataSource.h"
#include "DataPool/Vector/DataPoolLines.h"
#include "DataPool/Vector/DataPoolPoints.h"
#include "DataPool/Vector/DataPoolPolygons.h"
#include "DataPool.generated.h"

class UDataPool;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDataPoolSetupFinishedMulticastDelegate, UDataPool*, a_dataPool);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDataSourceDataUpdatedMulticastDelegate, const FName&, a_dataSourceName);

class ShapeData;
class UConfig;
class IDataPoolRasterReadOnly;
class URasterLayer;
class UVectorLayer;
class FMSPConfigFilePathResolver;
class UDataPoolRaster;
struct FRasterTypeMapping;

UCLASS()
class UDataPool : public UObject
{
	GENERATED_BODY()
public:
	friend class ADataPoolTickActor;

	void Setup(const FMSPConfigFilePathResolver& a_pathResolver, const UConfig* a_config);

	void RegisterDataPoolSetupFinishedListener(const FOnDataPoolSetupFinishedMulticastDelegate::FDelegate& a_listener);
	void UnregisterDataPoolSetupFinishedListener(const FOnDataPoolSetupFinishedMulticastDelegate::FDelegate& a_listener);

	bool TryRegisterDataUpdatedListener(const FName& a_dataSourceName, const FOnDataSourceDataUpdatedMulticastDelegate::FDelegate& a_listener);
	bool TryUnRegisterDataUpdatedListener(const FName& a_dataSourceName, const FOnDataSourceDataUpdatedMulticastDelegate::FDelegate& a_listener);

	bool TryGetData(const FName& a_dataSourceName, float& a_data) const;
	bool TryGetData(const FName& a_dataSourceName, IDataPoolRasterReadOnly*& a_data) const;
	bool TryGetData(const FName& a_dataSourceName, FDataPoolLines& a_data) const;
	bool TryGetData(const FName& a_dataSourceName, FDataPoolPoints& a_data) const;
	bool TryGetData(const FName& a_dataSourceName, FDataPoolPolygons& a_data) const;
	bool TryGetData(const FName& a_dataSourceName, FString& a_data) const;

	bool TrySetData(const FName& a_dataSourceName, const float a_data);
	bool TrySetData(const FName& a_dataSourceName, TStrongObjectPtr<UDataPoolRaster> a_data);
	bool TrySetData(const FName& a_dataSourceName, FDataPoolLines a_data);
	bool TrySetData(const FName& a_dataSourceName, FDataPoolPoints a_data);
	bool TrySetData(const FName& a_dataSourceName, FDataPoolPolygons a_data);
	bool TrySetData(const FName& a_dataSourceName, const FString& a_data);

	bool CreateDataSource(const FName& a_dataSourceName, const EDataPoolDataType a_dataType, const EDataPoolDataUnitType a_dataUnitType);
	bool CreateDataSource(const FName& a_dataSourceName, float a_data, const EDataPoolDataUnitType a_dataUnitType);
	bool CreateDataSource(const FName& a_dataSourceName, UDataPoolRaster* a_data, const EDataPoolDataUnitType a_dataUnitType);
	bool CreateDataSource(const FName& a_dataSourceName, FDataPoolLines a_data, const EDataPoolDataUnitType a_dataUnitType);
	bool CreateDataSource(const FName& a_dataSourceName, FDataPoolPoints a_data, const EDataPoolDataUnitType a_dataUnitType);
	bool CreateDataSource(const FName& a_dataSourceName, FDataPoolPolygons a_data, const EDataPoolDataUnitType a_dataUnitType);
	bool CreateDataSource(const FName& a_dataSourceName, const FString& a_data, const EDataPoolDataUnitType a_dataUnitType);

	TArray<FName> GetDataSourceNames() const;
	bool GetDataType(const FName& a_dataSourceName, EDataPoolDataType& a_dataType, FString* a_errorMsg = nullptr) const;
	bool GetDataUnitType(const FName& a_dataSourceName, EDataPoolDataUnitType& a_dataUnitType, FString* a_errorMsg = nullptr) const;
	EDataPoolDataSourceLoadedType GetLoadedTypeForDataSource(const FName& a_dataSourceName) const;
	bool GetDataTypesForDataSource(const FName& a_dataSourceName, UDataPoolDataSourceTypeCollection*& a_typeCollection) const;
	bool IsDataSourceValid(const FName& a_dataSourceName, FString* a_errorMsg = nullptr) const;

	const FBox2D& GetSimulationBounds() const;

private:
	void NotifyDataSourceUpdated(const FName& a_dataSourceName);

	void SetupDataSourcesWithDefaults();
	void SetupDataSourcesFromJson(const FMSPConfigFilePathResolver& a_pathResolver, const UConfig* a_config);
	void SetupDataSourceFromLocalFile(const FMSPConfigFilePathResolver& a_pathResolver, const UConfig* a_config);
	void SetupPointsSourceFromConfig(const FName& a_dataSourceName, UVectorLayer* a_vectorLayer);
	void SetupLinesSourceFromConfig(const FName& a_dataSourceName, UVectorLayer* a_vectorLayer);
	void SetupPolygonsSourceFromConfig(const FName& a_dataSourceName, UVectorLayer* a_vectorLayer);
	bool SetupVectorSourceFromConfig(UVectorLayer* a_vectorLayer);

	bool SetupRasterSourceFromConfig(
		const FMSPConfigFilePathResolver& a_pathResolver,
		const FString& a_rasterLayerName,
		FName a_rasterDataSourceName,
		const FString& a_rasterLayerData,
		const FVector2D& a_rasterCoordinate0,
		const FVector2D& a_rasterCoordinate1,
		const FRasterValueScalingData& a_rasterValueScalingData,
		const FRasterTypeMapping& a_rasterTypeMapping,
		const FVector2D& a_configDataModelWorldOffset
	);
	void SetupLayerTypesFromConfig(const UALayer& a_layerFromConfig, const EDataPoolDataType a_dataType);

	FName GetTranslatedDataSourceNameFromConfig(const UALayer& a_layerFromConfig) const;

	void ProcessChangedEvents();
	void SynchronizedSetData(TFunction<void(UDataPool*)> SetFunction);

	UPROPERTY()
		FOnDataPoolSetupFinishedMulticastDelegate m_dataPoolSetupFinishedSubscriptions;
	UPROPERTY()
		TMap<FName, FOnDataSourceDataUpdatedMulticastDelegate> m_dataUpdatedSubscriptionsMap;

	UPROPERTY()
		TMap<FName, float> m_floatContainer;
	UPROPERTY()
		TMap<FName, FDataPoolLines> m_linesContainer;
	UPROPERTY()
		TMap<FName, FDataPoolPoints> m_pointsContainer;
	UPROPERTY()
		TMap<FName, FDataPoolPolygons> m_polygonsContainer;
	UPROPERTY()
		TMap<FName, UDataPoolRaster*> m_rasterContainer;
	UPROPERTY()
		TMap<FName, FString> m_stringContainer;

	UPROPERTY()
		TMap<FName, FDataPoolDataSource> m_dataSources;
	TArray<TFunction<void(UDataPool*)>> m_dataUpdatedCallbackQueue;
	FCriticalSection m_dataUpdatedCallbackQueueMutex;

	//We need the configured bounds so we can setup a 'sensible' default for the raster layer projections.
	FBox2D m_simulatedBounds;

	bool m_setupFinished = false;

};
