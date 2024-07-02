#pragma once

#include "DataPoolDataSourceTypeCollection.h"
#include "DataPool/DataPool.h"
#include "DataPool/DataPoolDataSourceRef.h"
#include "DataPool/DataPoolDataTypes.h"
#include "DataPool/DataPoolDataSourceLoadedType.h"
#include "DataPool/DataPoolDataUnitTypes.h"
#include "DataPoolBlueprintFunctions.generated.h"

class ShapeData;
class UDataPool;
class IDataPoolRasterReadOnly;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnDataPoolSetupFinishedDelegate, UDataPool*, DataPool);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnDataSourceDataUpdatedDelegate, const FName&, a_dataSourceName);

UENUM()
enum class EDataPoolOperationResult: uint8
{
	Success,
	Failure
};

UCLASS(BlueprintType)
class UDataPoolBlueprintFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	// general
	UFUNCTION(BlueprintCallable, Category="Data pool", meta = (ToolTip="Get available data sources"))
		static TArray<FName> GetDataSourceNames();
	UFUNCTION(BlueprintCallable, Category="Data pool", meta = (ToolTip="Get the data type of specified data source", ExpandEnumAsExecs = "ReturnValue"))
		static EDataPoolOperationResult TryGetDataType(const FDataPoolDataSourceRef& DataSourceReference, EDataPoolDataType& DataType);
	UFUNCTION(BlueprintCallable, Category="Data pool", meta = (ToolTip="Get the data unit type of specified data source", ExpandEnumAsExecs = "ReturnValue"))
		static EDataPoolOperationResult TryGetDataUnitType(const FDataPoolDataSourceRef& DataSourceReference, EDataPoolDataUnitType& DataUnitType);
	UFUNCTION(BlueprintCallable, Category="Data pool", meta = (ToolTip="Get the loading type of specified data source", ExpandEnumAsExecs = "ReturnValue"))
		static EDataPoolDataSourceLoadedType GetDataSourceLoadedType(const FDataPoolDataSourceRef& DataSourceReference);
	UFUNCTION(BlueprintCallable, Category="Data pool", meta = (ToolTip="Get the data source name"))
		static const FName& GetDataSourceName(const EDataPoolDataSource DataSource);
	UFUNCTION(BlueprintCallable, Category="Data pool", meta = (ToolTip="Gets the types associated with this layer", ExpandEnumAsExecs = "ReturnValue"))
		static EDataPoolOperationResult TryGetDataTypes(const FDataPoolDataSourceRef& DataSourceReference, UDataPoolDataSourceTypeCollection*& SourceTypes);

	// string data
	UFUNCTION(BlueprintCallable, Category="Data pool|String data source", meta = (ToolTip="Get data from specified string data source", ExpandEnumAsExecs = "ReturnValue"))
		static EDataPoolOperationResult TryGetString(const FDataPoolDataSourceRef& DataSourceReference, FString& StringData);
	UFUNCTION(BlueprintCallable, Category="Data pool|String data source", meta = (ToolTip="Set data of specified string data source", ExpandEnumAsExecs = "ReturnValue"))
		static EDataPoolOperationResult TrySetString(const FDataPoolDataSourceRef& DataSourceReference, const FString& StringData);

	// float data
	UFUNCTION(BlueprintCallable, Category="Data pool|Float data source", meta = (ToolTip="Get data from specified float data source", ExpandEnumAsExecs = "ReturnValue"))
		static EDataPoolOperationResult TryGetFloat(const FDataPoolDataSourceRef& DataSourceReference, float& FloatData);
	UFUNCTION(BlueprintCallable, Category="Data pool|Float data source", meta = (ToolTip="Set data of specified float data source", ExpandEnumAsExecs = "ReturnValue"))
		static EDataPoolOperationResult TrySetFloat(const FDataPoolDataSourceRef& DataSourceReference, const float FloatData);

	// raster data
	UFUNCTION(BlueprintCallable, Category="Data pool|Raster data source", meta = (ToolTip="Get data from specified raster data source", ExpandEnumAsExecs = "ReturnValue"))
		static EDataPoolOperationResult TryGetRaster(const FDataPoolDataSourceRef& DataSourceReference, TScriptInterface<IDataPoolRasterReadOnly>& a_rasterData);
	UFUNCTION(BlueprintCallable, Category="Data pool|Raster data source", meta = (ToolTip="Get data from specified raster data source", ExpandEnumAsExecs = "ReturnValue"))
		static EDataPoolOperationResult TrySetRaster(const FDataPoolDataSourceRef& DataSourceReference, UDataPoolRaster* RasterData);

	// vector data
	UFUNCTION(BlueprintCallable, Category="Data pool|Vector data sources|Lines data source", meta = (ToolTip="Get data from lines data source", ExpandEnumAsExecs = "ReturnValue"))
		static EDataPoolOperationResult TryGetLines(const FDataPoolDataSourceRef& DataSourceReference, FDataPoolLines& Lines);
	UFUNCTION(BlueprintCallable, Category="Data pool|Vector data sources|Lines data source", meta = (ToolTip="Set data of specified lines data source", ExpandEnumAsExecs = "ReturnValue"))
		static EDataPoolOperationResult TrySetLines(const FDataPoolDataSourceRef& DataSourceReference, const FDataPoolLines& Lines);
	UFUNCTION(BlueprintCallable, Category="Data pool|Vector data sources|Points data source", meta = (ToolTip="Get data from points data source", ExpandEnumAsExecs = "ReturnValue"))
		static EDataPoolOperationResult TryGetPoints(const FDataPoolDataSourceRef& DataSourceReference, FDataPoolPoints& Points);
	UFUNCTION(BlueprintCallable, Category="Data pool|Vector data sources|Points data source", meta = (ToolTip="Set data of specified points data source", ExpandEnumAsExecs = "ReturnValue"))
		static EDataPoolOperationResult TrySetPoints(const FDataPoolDataSourceRef& DataSourceReference, const FDataPoolPoints& Points);
	UFUNCTION(BlueprintCallable, Category="Data pool|Vector data sources|Polygons data source", meta = (ToolTip="Get data from polygons data source", ExpandEnumAsExecs = "ReturnValue"))
		static EDataPoolOperationResult TryGetPolygons(const FDataPoolDataSourceRef& DataSourceReference, FDataPoolPolygons& Polygons);
	UFUNCTION(BlueprintCallable, Category="Data pool|Vector data sources|Polygons data source", meta = (ToolTip="Set data of specified polygons data source", ExpandEnumAsExecs = "ReturnValue"))
		static EDataPoolOperationResult TrySetPolygons(const FDataPoolDataSourceRef& DataSourceReference, const FDataPoolPolygons& Polygons);

	UFUNCTION(BlueprintCallable, Category="Data pool|Custom data sources", meta = (ToolTip="Create a custom data source", ExpandEnumAsExecs = "ReturnValue"))
		static EDataPoolOperationResult CreateDataSource(const FName DataSource, const EDataPoolDataType DataType, const EDataPoolDataUnitType DataUnitType);

	UFUNCTION(BlueprintCallable, Category="Data pool")
		static void RegisterDataPoolSetupFinished(FOnDataPoolSetupFinishedDelegate CallbackEvent);

	UFUNCTION(BlueprintCallable, Category="Data pool", meta = (ExpandEnumAsExecs = "ReturnValue"))
		static EDataPoolOperationResult TryRegisterDataUpdatedCallback(const FDataPoolDataSourceRef& DataSourceName, const FOnDataSourceDataUpdatedDelegate& CallbackEvent);

	UFUNCTION(BlueprintCallable, Category="Data pool", meta = (ExpandEnumAsExecs = "ReturnValue"))
		static EDataPoolOperationResult TryUnRegisterDataUpdatedCallback(const FDataPoolDataSourceRef& DataSourceName, const FOnDataSourceDataUpdatedDelegate& CallbackEvent);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		static FDataPoolLine CreateLineFromPoints(TArray<FVector> Points, TMap<FString, FString> MetaData);

	UFUNCTION(BlueprintCallable)
		static UPARAM(ref) FDataPoolPoints& UpdateMetaDataOnPoint(UPARAM(ref) FDataPoolPoints& Points, FString TargetMetaKey, FString TargetMetaValue, TMap<FString, FString> MetaDataToSet, bool& Success);
	UFUNCTION(BlueprintCallable)
		static UPARAM(ref) FDataPoolPoints& UpdatePoint(UPARAM(ref)FDataPoolPoints& Points, FString TargetMetaKey, FString TargetMetaValue, FVector NewLocation, TMap<FString, FString> MetaDataToSet, bool& Success);
	UFUNCTION(BlueprintCallable)
		static UPARAM(ref) FDataPoolPoints& AddPoint(UPARAM(ref)FDataPoolPoints& Points, FVector NewLocation, TMap<FString, FString> MetaDataToSet, bool& Success);
	UFUNCTION(BlueprintCallable)
		static UPARAM(ref) FDataPoolPoints& RemovePoint(UPARAM(ref)FDataPoolPoints& Points, FString TargetMetaKey, FString TargetMetaValue, bool& Success);

	UFUNCTION(BlueprintCallable)
	static bool IsWithEditor();

	UFUNCTION(BlueprintCallable)
	static FBox2D GetSimulatedBounds();

private:
	static UDataPool& GetDataPool();
};
