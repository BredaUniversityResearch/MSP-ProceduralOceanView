#pragma once

#include "Config/VectorLayer.h"
#include "Config/VectorObject.h"
#include "DataPool/DataPoolDataSourceTypeCollection.h"
#include "DataPoolVectorShape.generated.h"

USTRUCT(Blueprintable)
struct FDataPoolVectorShape
{
	GENERATED_BODY()
	virtual ~FDataPoolVectorShape() = default;

	void SetupFromConfig(
		const UVectorLayer* a_configLayer,
		const UVectorObject* a_configData,
		const UDataPoolDataSourceTypeCollection* a_layerTypes
	) {
		m_metadata = a_configData->metadata->m_values; // make a copy
		if (a_layerTypes) {
			for (const int32 typeIndex : a_configData->types) {
				FDataPoolDataSourceType layerType;
				if (!a_layerTypes->FindTypeByIndex(typeIndex, layerType)) {
					continue;
				}
				m_layerTypes.Add(layerType);
			}
		}
		OnSetupFromConfig(a_configLayer, a_configData);
	}

	virtual void OnSetupFromConfig(const UVectorLayer* a_configLayer, const UVectorObject* a_configData)
	{
		// can be overriden by child struct to add more setup
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Data pool|Vector data sources", Meta = (DisplayName="Meta data"))
		TMap<FString, FString> m_metadata;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Data pool|Vector data sources", Meta = (DisplayName="Layer types"))
		TArray<FDataPoolDataSourceType> m_layerTypes;
};
