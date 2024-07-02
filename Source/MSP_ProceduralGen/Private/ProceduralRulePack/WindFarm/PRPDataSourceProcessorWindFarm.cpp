#include "PRPDataSourceProcessorWindFarm.h"

#include "DataPool/DataPool.h"
#include "DataPool/VectorDataUtilities.h"
#include "DataPool/DataPoolDataSourceNames.h"
#include "DataPool/Vector/DataPoolPolygons.h"
#include "MSP_ProceduralGen/MSP_ProceduralGen.h"


void UPRPDataSourceProcessorWindFarm::RegisterWithDataPool_Implementation(UDataPool* DataPool)
{
	Super::RegisterWithDataPool_Implementation(DataPool);

	ensure(DataPool != nullptr);

	m_dataPoolChangedDelegate.BindDynamic(this, &UPRPDataSourceProcessorWindFarm::OnDataSourceChanged);
	const bool result = DataPool->TryRegisterDataUpdatedListener(DataPoolDataSourceNames::WIND_PARK, m_dataPoolChangedDelegate);
	check(result);

	bool createdDataLayer = DataPool->CreateDataSource(WindFarmPlacementDataSourceName, EDataPoolDataType::Points, EDataPoolDataUnitType::Other);
	check(createdDataLayer)
	createdDataLayer = DataPool->CreateDataSource(WindFarmTransformerPlacementDataSourceName, EDataPoolDataType::Points, EDataPoolDataUnitType::Other);
	check(createdDataLayer)

	FDataPoolLines cableLayer;
	if (!DataPool->TryGetData(DataPoolDataSourceNames::ENERGY_CABLES, cableLayer))
	{
		UE_LOG(LogMspProceduralGen, Warning, TEXT("Could not find data Energy Cable layer in the data pool."));
	}

	FDataPoolPolygons windFarmLayer;
	if (DataPool->TryGetData(DataPoolDataSourceNames::WIND_PARK, windFarmLayer))
	{
		RecalculatePlacementData(windFarmLayer, cableLayer);
	}
}

void UPRPDataSourceProcessorWindFarm::OnDataSourceChanged(const FName& DataSourceName)
{
	if (DataSourceName != DataPoolDataSourceNames::WIND_PARK)
	{
		return;
	}

	FDataPoolPolygons polygonData;
	bool result = TargetDataPool->TryGetData(DataPoolDataSourceNames::WIND_PARK, polygonData);
	ensure(result);

	FDataPoolLines energyCablesData;
	result = TargetDataPool->TryGetData(DataPoolDataSourceNames::ENERGY_CABLES, energyCablesData);
	ensure(result);

	RecalculatePlacementData(polygonData, energyCablesData);
}

void UPRPDataSourceProcessorWindFarm::RecalculatePlacementData(const FDataPoolPolygons& PolygonData, const FDataPoolLines& EnergyCablesData) const
{
	FDataPoolPoints windFarmPlacementData = {};
	FDataPoolPoints windFarmTransformerPlacementData = {};

	FDataPoolPoints windFarmNoiseSourceData = {};
	bool result = TargetDataPool->TryGetData(DataPoolDataSourceNames::NOISE_SOURCE, windFarmNoiseSourceData);
	ensure(result);
	ensure(windFarmNoiseSourceData.m_points.Num() == 0); //If this is not correct we need to look up the existing values in the loop, or completely discard any existing values which runs us the risk of actually nuking values that are not associated with the wind farm data.

	int windFarmInstanceId = 0;
	int windTurbineUniqueId = 0;
	for (const FDataPoolPolygon& shape : PolygonData.m_polygons)
	{
		FString windFarmInstanceIdString = FString::FromInt(windFarmInstanceId);
		++windFarmInstanceId;

		int objectsToPlace = 0;
		if (const FString* metaDataField = shape.m_metadata.Find(WindFarmAmountMetaDataField))
		{
			objectsToPlace = FPlatformString::Atoi(**metaDataField);
		}
		else
		{
			UE_LOG(LogMspProceduralGen, Warning, TEXT("Layer (%s) entity did not report any metadata for amount in field %s."), *DataPoolDataSourceNames::WIND_PARK.ToString(), *WindFarmAmountMetaDataField);
		}

		TArray<TArray<FVector2D>> gaps = {};
		for (const FDataPoolVectorArrayWrapper& wrapper : shape.m_gaps)
		{
			TArray<FVector2D> gap;
			gap.AddUninitialized(wrapper.m_vectorArray.Num());
			for (int i = 0; i < wrapper.m_vectorArray.Num(); ++i)
			{
				gap[i] = FVector2D(wrapper.m_vectorArray[i].X, wrapper.m_vectorArray[i].Y);
			}
			gaps.Add(gap);
		}

		TArray<FVector2D> shapeAs2D;
		shapeAs2D.AddUninitialized(shape.m_points.m_vectorArray.Num());
		for (int i = 0; i < shape.m_points.m_vectorArray.Num(); ++i)
		{
			shapeAs2D[i] = FVector2D(shape.m_points.m_vectorArray[i].X, shape.m_points.m_vectorArray[i].Y);
		}

		FVector2D transformerPlacement = CalculateTransformerPlacement(EnergyCablesData, shapeAs2D, gaps);
		//Add a gap around the transformer to avoid turbines being placed too close to the transformer.
		gaps.Emplace(TArray{
			transformerPlacement + FVector2D(-TransformerSafeAreaHalfSize, -TransformerSafeAreaHalfSize),
			transformerPlacement + FVector2D( TransformerSafeAreaHalfSize, -TransformerSafeAreaHalfSize),
			transformerPlacement + FVector2D( TransformerSafeAreaHalfSize,  TransformerSafeAreaHalfSize),
			transformerPlacement + FVector2D(-TransformerSafeAreaHalfSize,  TransformerSafeAreaHalfSize)
		});

		TArray<FVector2D> placementPoints = FVectorDataUtilities::GeneratePointsIn2DPolygon(shapeAs2D, gaps, objectsToPlace, objectsToPlace > 0? 0.0f : WindfarmDefaultSpacing);

		CreateTransformerPlacement(transformerPlacement, windFarmInstanceIdString, placementPoints.Num(), windFarmTransformerPlacementData);

		FString NameMetaDataField = "default";
		if (const FString* fieldName = shape.m_metadata.Find(WindFarmInputNameMetaDataField))
		{
			NameMetaDataField = *fieldName;
		}

		TMap<FString, FString> windFarmMetaData = {
			{ WindFarmInstanceIdMetaDataField, windFarmInstanceIdString },
			{ WindFarmNameMetaDataField, NameMetaDataField }
		};

		TMap<FString, FString> windFarmNoiseMetaData = {
			{ WindFarmNoiseSourceMetaDataField, "Wind Turbine"},
			{ WindFarmNoiseSourceMetaDataIntensityField, "0" },
			{ WindFarmNoiseSourceMetaDataDistanceField,  "0" },
		};

		FString windTurbineUniqueIdString;
		for (FVector2D placementPoint : placementPoints)
		{
			windTurbineUniqueIdString = FString::FromInt(windTurbineUniqueId);
			++windTurbineUniqueId;

			windFarmNoiseMetaData.Add(WindFarmTurbineIdMetaDataField, windTurbineUniqueIdString);
			windFarmMetaData.Add(WindFarmTurbineIdMetaDataField, windTurbineUniqueIdString);

			FDataPoolPoint windFarmPoint = FDataPoolPoint(FVector(placementPoint, 0.0f));
			windFarmPoint.m_metadata = windFarmMetaData;
			windFarmPlacementData.m_points.Emplace(MoveTemp(windFarmPoint));

			FDataPoolPoint windFarmNoisePoint = FDataPoolPoint(FVector(placementPoint, 0.0f));
			windFarmNoisePoint.m_metadata = windFarmNoiseMetaData;

			windFarmNoiseSourceData.m_points.Emplace(MoveTemp(windFarmNoisePoint));

			windFarmNoiseMetaData.Remove(WindFarmTurbineIdMetaDataField);
			windFarmMetaData.Remove(WindFarmTurbineIdMetaDataField);
		}
	}
	TargetDataPool->TrySetData(DataPoolDataSourceNames::NOISE_SOURCE, MoveTemp(windFarmNoiseSourceData));
	TargetDataPool->TrySetData(WindFarmTransformerPlacementDataSourceName, MoveTemp(windFarmTransformerPlacementData));
	TargetDataPool->TrySetData(WindFarmPlacementDataSourceName, MoveTemp(windFarmPlacementData));
}

void UPRPDataSourceProcessorWindFarm::CreateTransformerPlacement(FVector2D TransformerPlacementLocation, const FString& WindFarmInstanceId, int NumConnectedTurbines, FDataPoolPoints& OutPlacementData) const
{
	TMap<FString, FString> transformerMetaData =
	{
		{ WindFarmInstanceIdMetaDataField, WindFarmInstanceId},
		{ NumOfConnectedTurbinesDataField, FString::FromInt(NumConnectedTurbines) }
	};

	FDataPoolPoint transformerPoint = FDataPoolPoint(FVector(TransformerPlacementLocation, 0.0f));
	transformerPoint.m_metadata = transformerMetaData;
	OutPlacementData.m_points.Emplace(MoveTemp(transformerPoint));
}

FVector2D UPRPDataSourceProcessorWindFarm::CalculateTransformerPlacement(const FDataPoolLines& EnergyCablesData, const TArray<FVector2D>& WindFarmShapeAs2D, const TArray<TArray<FVector2D>>& WindFarmShapeGaps) const
{
	//PdG 2024-02-01; For now it seems that we have 1 transformer for 1 polygon... 
	for (const FDataPoolLine& energyCableLine : EnergyCablesData.m_lines)
	{
		//Maybe we should consider only the first and last points of the line instead of all points?
		for (const FVector& energyCableLinePoint : energyCableLine.m_points.m_vectorArray)
		{
			FVector2D energyCableLinePoint2D = FVector2D(energyCableLinePoint.X, energyCableLinePoint.Y);
			if (FVectorDataUtilities::IsPointInsidePolygonWithHoles(energyCableLinePoint2D, WindFarmShapeAs2D, WindFarmShapeGaps))
			{
				return energyCableLinePoint2D;
			}
		}
	}

	//If we don't have an outgoing / incoming energy connection we place a transformer at the center 
	return FVectorDataUtilities::GetPolygonCenter(WindFarmShapeAs2D);
}
