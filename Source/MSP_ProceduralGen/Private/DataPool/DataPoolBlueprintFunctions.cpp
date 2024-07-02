#include "DataPool/DataPoolBlueprintFunctions.h"

#include "MSPProcGenGameInstance.h"
#include "Util.h"
#include "DataPool/DataPool.h"
#include "DataPool/DataPoolDataSourceTypeCollection.h"
#include "DataPool/DataPoolRaster.h"
#include "MSP_ProceduralGen/MSP_ProceduralGen.h"

TArray<FName> UDataPoolBlueprintFunctions::GetDataSourceNames()
{
	return GetDataPool().GetDataSourceNames();
}

EDataPoolOperationResult UDataPoolBlueprintFunctions::TryGetDataType(const FDataPoolDataSourceRef& DataSourceReference, EDataPoolDataType& DataType)
{
	return GetDataPool().GetDataType(DataSourceReference.Resolve(), DataType) ? EDataPoolOperationResult::Success : EDataPoolOperationResult::Failure;
}

EDataPoolOperationResult UDataPoolBlueprintFunctions::TryGetDataUnitType(const FDataPoolDataSourceRef& DataSourceReference, EDataPoolDataUnitType& DataUnitType)
{
	return GetDataPool().GetDataUnitType(DataSourceReference.Resolve(), DataUnitType) ? EDataPoolOperationResult::Success : EDataPoolOperationResult::Failure;
}

EDataPoolDataSourceLoadedType UDataPoolBlueprintFunctions::GetDataSourceLoadedType(const FDataPoolDataSourceRef& DataSourceReference)
{
	return GetDataPool().GetLoadedTypeForDataSource(DataSourceReference.Resolve());
}

const FName& UDataPoolBlueprintFunctions::GetDataSourceName(const EDataPoolDataSource DataSource)
{
	return DataPoolDataSourceNames::GetName(DataSource);
}

EDataPoolOperationResult UDataPoolBlueprintFunctions::TryGetDataTypes(const FDataPoolDataSourceRef& DataSourceReference, UDataPoolDataSourceTypeCollection*& SourceTypes)
{
	return (GetDataPool().GetDataTypesForDataSource(DataSourceReference.Resolve(), SourceTypes))? EDataPoolOperationResult::Success : EDataPoolOperationResult::Failure;
}

EDataPoolOperationResult UDataPoolBlueprintFunctions::TryGetString(const FDataPoolDataSourceRef& DataSourceReference, FString& StringData)
{
	return GetDataPool().TryGetData(DataSourceReference.Resolve(), StringData) ? EDataPoolOperationResult::Success : EDataPoolOperationResult::Failure;
}

EDataPoolOperationResult UDataPoolBlueprintFunctions::TrySetString(const FDataPoolDataSourceRef& DataSourceReference, const FString& StringData)
{
	return GetDataPool().TrySetData(DataSourceReference.Resolve(), StringData) ? EDataPoolOperationResult::Success : EDataPoolOperationResult::Failure;
}

EDataPoolOperationResult UDataPoolBlueprintFunctions::TryGetFloat(const FDataPoolDataSourceRef& DataSourceReference, float& FloatData)
{
	return GetDataPool().TryGetData(DataSourceReference.Resolve(), FloatData) ? EDataPoolOperationResult::Success : EDataPoolOperationResult::Failure;
}

EDataPoolOperationResult UDataPoolBlueprintFunctions::TrySetFloat(const FDataPoolDataSourceRef& DataSourceReference, const float FloatData)
{
	return GetDataPool().TrySetData(DataSourceReference.Resolve(), FloatData) ? EDataPoolOperationResult::Success : EDataPoolOperationResult::Failure;
}

EDataPoolOperationResult UDataPoolBlueprintFunctions::TryGetRaster(const FDataPoolDataSourceRef& DataSourceReference, TScriptInterface<IDataPoolRasterReadOnly>& a_rasterData)
{
	IDataPoolRasterReadOnly* iface{nullptr};
	bool success = GetDataPool().TryGetData(DataSourceReference.Resolve(), iface);

	if (success)
	{
		UDataPoolRaster* rasterLayer = dynamic_cast<UDataPoolRaster*>(iface);
		check(rasterLayer != nullptr);
		a_rasterData = TScriptInterface<IDataPoolRasterReadOnly>(rasterLayer);
	}

	return success ? EDataPoolOperationResult::Success : EDataPoolOperationResult::Failure;
}

EDataPoolOperationResult UDataPoolBlueprintFunctions::TrySetRaster(const FDataPoolDataSourceRef& DataSourceReference, UDataPoolRaster* RasterData)
{
	return GetDataPool().TrySetData(DataSourceReference.Resolve(), TStrongObjectPtr(RasterData)) ? EDataPoolOperationResult::Success : EDataPoolOperationResult::Failure;
}

EDataPoolOperationResult UDataPoolBlueprintFunctions::TryGetLines(const FDataPoolDataSourceRef& DataSourceReference, FDataPoolLines& Lines)
{
	return GetDataPool().TryGetData(DataSourceReference.Resolve(), Lines) ? EDataPoolOperationResult::Success : EDataPoolOperationResult::Failure;
}

EDataPoolOperationResult UDataPoolBlueprintFunctions::TrySetLines(const FDataPoolDataSourceRef& DataSourceReference, const FDataPoolLines& Lines)
{
	return GetDataPool().TrySetData(DataSourceReference.Resolve(), Lines) ? EDataPoolOperationResult::Success : EDataPoolOperationResult::Failure;
}

EDataPoolOperationResult UDataPoolBlueprintFunctions::TryGetPoints(const FDataPoolDataSourceRef& DataSourceReference, FDataPoolPoints& Points)
{
	return GetDataPool().TryGetData(DataSourceReference.Resolve(), Points) ? EDataPoolOperationResult::Success : EDataPoolOperationResult::Failure;
}

EDataPoolOperationResult UDataPoolBlueprintFunctions::TrySetPoints(const FDataPoolDataSourceRef& DataSourceReference, const FDataPoolPoints& Points)
{
	return GetDataPool().TrySetData(DataSourceReference.Resolve(), Points) ? EDataPoolOperationResult::Success : EDataPoolOperationResult::Failure;
}

EDataPoolOperationResult UDataPoolBlueprintFunctions::TryGetPolygons(const FDataPoolDataSourceRef& DataSourceReference, FDataPoolPolygons& Polygons)
{
	return GetDataPool().TryGetData(DataSourceReference.Resolve(), Polygons) ? EDataPoolOperationResult::Success : EDataPoolOperationResult::Failure;
}

EDataPoolOperationResult UDataPoolBlueprintFunctions::TrySetPolygons(const FDataPoolDataSourceRef& DataSourceReference, const FDataPoolPolygons& Polygons)
{
	return GetDataPool().TrySetData(DataSourceReference.Resolve(), Polygons) ? EDataPoolOperationResult::Success : EDataPoolOperationResult::Failure;
}

EDataPoolOperationResult UDataPoolBlueprintFunctions::CreateDataSource(const FName DataSource, const EDataPoolDataType DataType, const EDataPoolDataUnitType DataUnitType)
{
	return GetDataPool().CreateDataSource(DataSource, DataType, DataUnitType) ? EDataPoolOperationResult::Success : EDataPoolOperationResult::Failure;
}

void UDataPoolBlueprintFunctions::RegisterDataPoolSetupFinished(FOnDataPoolSetupFinishedDelegate CallbackEvent)
{
	GetDataPool().RegisterDataPoolSetupFinishedListener(CallbackEvent);
}

EDataPoolOperationResult UDataPoolBlueprintFunctions::TryRegisterDataUpdatedCallback(const FDataPoolDataSourceRef& DataSourceName, const FOnDataSourceDataUpdatedDelegate& CallbackEvent)
{
	return GetDataPool().TryRegisterDataUpdatedListener(DataSourceName.Resolve(), CallbackEvent) ? EDataPoolOperationResult::Success : EDataPoolOperationResult::Failure;
}

EDataPoolOperationResult UDataPoolBlueprintFunctions::TryUnRegisterDataUpdatedCallback(const FDataPoolDataSourceRef& DataSourceName, const FOnDataSourceDataUpdatedDelegate& CallbackEvent)
{
	return GetDataPool().TryUnRegisterDataUpdatedListener(DataSourceName.Resolve(), CallbackEvent) ? EDataPoolOperationResult::Success : EDataPoolOperationResult::Failure;
}

FDataPoolLine UDataPoolBlueprintFunctions::CreateLineFromPoints(TArray<FVector> Points, TMap<FString, FString> MetaData)
{
	FDataPoolLine result = FDataPoolLine(FDataPoolVectorArrayWrapper(MoveTemp(Points)));
	result.m_metadata = MoveTemp(MetaData);
	return result;
}

FDataPoolPoints& UDataPoolBlueprintFunctions::UpdateMetaDataOnPoint(FDataPoolPoints& Points, FString TargetMetaKey, FString TargetMetaValue, TMap<FString, FString> MetaDataToSet, bool& Success)
{
	Success = false;
	for(FDataPoolPoint& point: Points.m_points)
	{
		FString* targetMeta = point.m_metadata.Find(TargetMetaKey);
		if (targetMeta != nullptr && *targetMeta == TargetMetaValue)
		{
			point.m_metadata.Append(MetaDataToSet);
			Success = true;
			break;
		}
	}
	return Points;
}

FDataPoolPoints& UDataPoolBlueprintFunctions::UpdatePoint(FDataPoolPoints& Points, FString TargetMetaKey, FString TargetMetaValue, FVector NewLocation, TMap<FString, FString> MetaDataToSet, bool& Success)
{
	Success = false;
	for (FDataPoolPoint& point : Points.m_points)
	{
		FString* targetMeta = point.m_metadata.Find(TargetMetaKey);
		if (targetMeta != nullptr && *targetMeta == TargetMetaValue)
		{
			point.m_vector = NewLocation;
			point.m_metadata.Append(MetaDataToSet);
			Success = true;
			break;
		}
	}
	return Points;
}

FDataPoolPoints& UDataPoolBlueprintFunctions::AddPoint(FDataPoolPoints& Points, FVector NewLocation, TMap<FString, FString> MetaDataToSet, bool& Success)
{
	int index = Points.m_points.Emplace(NewLocation);
	Points.m_points[index].m_metadata.Append(MetaDataToSet);
	Success = true;
	return Points;
}

FDataPoolPoints& UDataPoolBlueprintFunctions::RemovePoint(FDataPoolPoints& Points, FString TargetMetaKey, FString TargetMetaValue, bool& Success)
{
	Success = false;

	for (int i = 0; i < Points.m_points.Num(); i+=1)
	{
		FDataPoolPoint& point = Points.m_points[i];
		FString* targetMeta = point.m_metadata.Find(TargetMetaKey);
		if (targetMeta != nullptr && *targetMeta == TargetMetaValue)
		{
			Points.m_points.RemoveAt(i);
			Success = true;
			break;
		}
	}
	return Points;
}

bool UDataPoolBlueprintFunctions::IsWithEditor()
{
#if WITH_EDITOR
	return true;
#else
	return false;
#endif
}

FBox2D UDataPoolBlueprintFunctions::GetSimulatedBounds()
{
	return GetDataPool().GetSimulationBounds();
}

UDataPool& UDataPoolBlueprintFunctions::GetDataPool()
{
	return UMSPProcGenGameInstance::GetInstance()->GetDataPool();
}
