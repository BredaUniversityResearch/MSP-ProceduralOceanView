#pragma once

#include "DataPool/DataPool.h"
#include "ProceduralRulePack/ProceduralRulePackDataSourceProcessor.h"
#include "PRPDataSourceProcessorWindFarm.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UPRPDataSourceProcessorWindFarm: public UProceduralRulePackDataSourceProcessor
{
	GENERATED_BODY()
	static constexpr double TransformerSafeAreaHalfSize = 10000; //Safe area around transformer placement where no wind turbines may be placed.
	static constexpr double WindfarmDefaultSpacing = 50000; //Default spacing for wind turbines when no amount is specified in the metadata.

public:
	static inline const FName WindFarmPlacementDataSourceName = TEXT("WindFarmPlacement");
	static inline const FName WindFarmTransformerPlacementDataSourceName = TEXT("WindFarmTransformerPlacement");
	static inline const FString WindFarmInstanceIdMetaDataField = TEXT("WindFarmInstanceId");
	static inline const FString WindFarmNameMetaDataField = TEXT("WindFarmName");
	static inline const FString WindFarmTurbineIdMetaDataField = TEXT("WindFarmTurbineId");
	static inline const FString WindFarmNoiseSourceMetaDataField = TEXT("NoiseSource");
	static inline const FString WindFarmNoiseSourceMetaDataIntensityField = TEXT("WindFarmNoiseIntensity");
	static inline const FString WindFarmNoiseSourceMetaDataDistanceField = TEXT("WindFarmNoiseFalloutDistance");
	static inline const FString NumOfConnectedTurbinesDataField = TEXT("NumOfConnectedTurbines");

	void RegisterWithDataPool_Implementation(UDataPool* DataPool) override;

	UFUNCTION()
	void OnDataSourceChanged(const FName& DataSourceName);

private:
	void RecalculatePlacementData(const FDataPoolPolygons& PolygonData, const FDataPoolLines& EnergyCablesData) const;

	void CreateTransformerPlacement(FVector2D TransformerPlacementLocation, const FString& WindFarmInstanceId, int NumConnectedTurbines, FDataPoolPoints& OutPlacementData) const;
	FVector2D CalculateTransformerPlacement(const FDataPoolLines& EnergyCablesData, const TArray<FVector2D>& WindFarmShapeAs2D, const TArray<TArray<FVector2D>>& WindFarmShapeGaps) const;

	FOnDataSourceDataUpdatedMulticastDelegate::FDelegate m_dataPoolChangedDelegate;

	UPROPERTY(EditDefaultsOnly)
	FString WindFarmAmountMetaDataField = "N_TURBINES";
	FString WindFarmInputNameMetaDataField = "name";
};