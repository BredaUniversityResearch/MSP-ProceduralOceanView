#pragma once

#include "DataPoolDataSourceLayerMatching.h"
#include "DataPoolDataTypes.h"
#include "DataPoolDataUnitTypes.h"
#include "DataPoolDataSourceLoadedType.h"
#include "Config/ALayer.h"
#include "DataPoolDataSource.generated.h"

class UDataPoolDataSourceTypeCollection;
class UALayer;

USTRUCT()
struct FDataPoolDataSource
{
	GENERATED_BODY()

	FDataPoolDataSource() = default;

	FDataPoolDataSource(const EDataPoolDataType a_dataType, const EDataPoolDataUnitType a_dataUnitType, UDataPoolDataSourceTypeCollection* a_layerTypes)
        : m_dataType(a_dataType)
		, m_dataUnitType(a_dataUnitType)
		, m_layerTypes(a_layerTypes)
	{
		check(m_layerTypes != nullptr);
	}

	EDataPoolDataSourceLoadedType GetLoadedType() const
	{
		return m_loadedType;
	}

	FDataPoolDataSource& SetLoadedType(const EDataPoolDataSourceLoadedType a_loadedType)
	{
		m_loadedType = a_loadedType;
		return *this;
	}

	bool IsMatchingLayer(const UALayer& a_layerFromConfig) const
	{
		// no data to match against
		if (m_layerMatching.m_inclLayerTags.Num() == 0 &&
			m_layerMatching.m_layerNamePartialMatchingEnabled == false) {
			return false;
		}

		// check if all including tags are in the layer
		for (const FName& tag : m_layerMatching.m_inclLayerTags)
		{
			if (!a_layerFromConfig.tags.Contains(tag))
			{
				return false;
			}
		}

		// check if all excluding tags are not in the layer
		for (const FName& tag : m_layerMatching.m_exclLayerTags)
		{
			if (a_layerFromConfig.tags.Contains(tag))
			{
				return false;
			}
		}

		// if partial matching is enabled, check if the layer name contains the partial match string
		if (m_layerMatching.m_layerNamePartialMatchingEnabled)
		{
			return a_layerFromConfig.name.Contains(m_layerMatching.m_layerNamePartialMatch);
		}

		// all tags matched!
		return true;
	}

	UPROPERTY()
	EDataPoolDataType m_dataType;
	UPROPERTY()
	EDataPoolDataUnitType m_dataUnitType;
	UPROPERTY()
	UDataPoolDataSourceTypeCollection* m_layerTypes;
	UPROPERTY()
	FDataPoolDataSourceLayerMatching m_layerMatching;
	UPROPERTY()
	EDataPoolDataSourceLoadedType m_loadedType = EDataPoolDataSourceLoadedType::Default;

	bool HasChanged = true;
};
