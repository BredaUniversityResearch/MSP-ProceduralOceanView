#include "DataPool/DataPoolDataSourceTypeCollection.h"

#include "Config/Common/StringMap.h"

UDataPoolDataSourceTypeCollection::UDataPoolDataSourceTypeCollection(const FObjectInitializer&)
{
}

void UDataPoolDataSourceTypeCollection::AddTypesFromConfig(const TArray<UStringMap*> a_configValues)
{
	verifyf(AvailableTypes.IsEmpty(), TEXT("Expected empty types to setup from config. If it's not empty the type indexing might be off"));
	for(int i = 0; i < a_configValues.Num(); ++i)
	{
		const UStringMap* map = a_configValues[i];

		FDataPoolDataSourceType type;
		type.TypeIndex = i;
		for (const auto& kvp : map->m_values)
		{
			if (kvp.Key == "name")
			{
				type.Name = FName(kvp.Value);
			}
			else
			{
				type.OtherValues.Add(kvp.Key, kvp.Value);
			}
		}
		AvailableTypes.Emplace(MoveTemp(type));
	}
}

bool UDataPoolDataSourceTypeCollection::FindTypeByIndex(int32 TypeIndex, FDataPoolDataSourceType& Result) const
{
	const FDataPoolDataSourceType* it = AvailableTypes.FindByPredicate([TypeIndex](const auto& a_entry) { return a_entry.TypeIndex == TypeIndex; });
	if (it != nullptr)
	{
		Result = *it;
		return true;
	}
	return false;
}

bool UDataPoolDataSourceTypeCollection::FindTypeByName(FName TypeName, FDataPoolDataSourceType& Result) const
{
	const FDataPoolDataSourceType* it = AvailableTypes.FindByPredicate([TypeName](const auto& a_entry) { return a_entry.Name == TypeName; });
	if (it != nullptr)
	{
		Result = *it;
		return true;
	}
	return false;
}
