#pragma once

#include "DataPoolDataSourceNames.h"
#include "DataPoolDataSourceRef.generated.h"

UENUM(BlueprintType)
enum class EDataPoolDataSourceRefType : uint8
{
	CustomName,
	PredefinedEnum
};

USTRUCT(BlueprintType)
struct FDataPoolDataSourceRef
{
	GENERATED_BODY()

	static const EDataPoolDataSource DEFAULT_DATA_SOURCE = EDataPoolDataSource::CloudCoverage;

	FDataPoolDataSourceRef() = default;

	// ReSharper disable once CppPossiblyUninitializedMember
	explicit FDataPoolDataSourceRef(const FString& a_customDataSourceName);

	explicit FDataPoolDataSourceRef(EDataPoolDataSource a_dataSource);

    bool operator==(const EDataPoolDataSource& a_dataSource) const;

	bool operator==(const FString& a_customDataSourceName) const;

	FName Resolve() const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Data pool|Data source reference", Meta = (DisplayName="Reference type"))
		EDataPoolDataSourceRefType m_refType = EDataPoolDataSourceRefType::PredefinedEnum;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Data pool|Data source reference", Meta = (DisplayName="Predefined enum"))
		EDataPoolDataSource m_dataSource = DEFAULT_DATA_SOURCE; // just pick a default one

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Data pool|Data source reference", Meta = (DisplayName="Set custom name"))
		FName m_customDataSourceName = "unused by default";
};
