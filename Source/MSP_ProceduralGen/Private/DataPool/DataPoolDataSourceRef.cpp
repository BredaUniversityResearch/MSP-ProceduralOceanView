#include "DataPool/DataPoolDataSourceRef.h"

FDataPoolDataSourceRef::FDataPoolDataSourceRef(const FString& a_customDataSourceName)
	: m_refType(EDataPoolDataSourceRefType::CustomName), m_customDataSourceName(a_customDataSourceName)
{
}

FDataPoolDataSourceRef::FDataPoolDataSourceRef(EDataPoolDataSource a_dataSource)
	: m_dataSource(a_dataSource)
{
}

bool FDataPoolDataSourceRef::operator==(const EDataPoolDataSource& a_dataSource) const
{
	if (m_refType != EDataPoolDataSourceRefType::PredefinedEnum) return false;
	return a_dataSource == m_dataSource;
}

bool FDataPoolDataSourceRef::operator==(const FString& a_customDataSourceName) const
{
	if (m_refType != EDataPoolDataSourceRefType::CustomName) return false;
	return a_customDataSourceName == m_customDataSourceName;
}

FName FDataPoolDataSourceRef::Resolve() const
{
	switch (m_refType) {
	case EDataPoolDataSourceRefType::PredefinedEnum:
		return DataPoolDataSourceNames::GetName(m_dataSource);
	case EDataPoolDataSourceRefType::CustomName:
		return m_customDataSourceName;
	default:
		return DataPoolDataSourceNames::GetName(DEFAULT_DATA_SOURCE);
	}
}
