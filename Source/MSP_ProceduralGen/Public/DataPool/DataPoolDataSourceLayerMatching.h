#pragma once

#include "DataPoolDataSourceLayerMatching.generated.h"

USTRUCT()
struct FDataPoolDataSourceLayerMatching
{
	GENERATED_BODY()

	explicit FDataPoolDataSourceLayerMatching(
		const TArray<FName>& a_inclLayerTags = {},
		const FString& a_layerNamePartialMatch = "",
		const TArray<FName>& a_exclLayerTags = {}
	)
		: m_inclLayerTags(a_inclLayerTags),
		  m_exclLayerTags(a_exclLayerTags),
	      m_layerNamePartialMatchingEnabled(a_layerNamePartialMatch.Len() > 0),
		  m_layerNamePartialMatch(a_layerNamePartialMatch)
	{
	}

	FDataPoolDataSourceLayerMatching& SetInclLayerTags(const TArray<FName>& a_inclLayerTags)
	{
		m_inclLayerTags = a_inclLayerTags;
		return *this;
	}

	FDataPoolDataSourceLayerMatching& SetExclLayerTags(const TArray<FName>& a_exclLayerTags)
	{
		m_exclLayerTags = a_exclLayerTags;
		return *this;
	}

	FDataPoolDataSourceLayerMatching& SetLayerNamePartialMatch(const FString& a_layerNamePartialMatch)
	{
		m_layerNamePartialMatchingEnabled = true;
		m_layerNamePartialMatch = a_layerNamePartialMatch;
		return *this;
	}

	UPROPERTY()
	TArray<FName> m_inclLayerTags;
	UPROPERTY()
	TArray<FName> m_exclLayerTags;
	UPROPERTY()
	bool m_layerNamePartialMatchingEnabled = false;
	UPROPERTY()
	FString m_layerNamePartialMatch;
};
