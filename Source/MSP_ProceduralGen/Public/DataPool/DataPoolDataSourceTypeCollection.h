#pragma once

#include "DataPoolDataSourceTypeCollection.generated.h"

class UStringMap;

USTRUCT(BLueprintType)
struct FDataPoolDataSourceType
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="Data pool|Vector data sources|Data source type", Meta = (DisplayName="Name"))
	FName Name;
	UPROPERTY()
	int32 TypeIndex;
	UPROPERTY(BlueprintReadOnly, Category="Data pool|Vector data sources|Data source type", Meta = (DisplayName="Map of name-value pairs"))
	TMap<FString, FString> OtherValues;
};

UCLASS()
class UDataPoolDataSourceTypeCollection: public UObject
{
	GENERATED_BODY()
public:
	UDataPoolDataSourceTypeCollection(const FObjectInitializer&);

	void AddTypesFromConfig(const TArray<UStringMap*> a_configValues);

	UFUNCTION(BlueprintCallable, BlueprintPure = false, meta=(ExpandBoolAsExecs="ReturnValue"))
	bool FindTypeByIndex(int32 TypeIndex, FDataPoolDataSourceType& Result) const;
	UFUNCTION(BlueprintCallable, BlueprintPure = false, meta=(ExpandBoolAsExecs="ReturnValue"))
	bool FindTypeByName(FName TypeName, FDataPoolDataSourceType& Result) const;

private:
	TArray<FDataPoolDataSourceType> AvailableTypes;
};
