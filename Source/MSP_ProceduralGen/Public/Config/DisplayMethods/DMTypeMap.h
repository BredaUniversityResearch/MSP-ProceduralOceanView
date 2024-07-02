//#pragma once
//
//#include "UObject/Object.h"
//#include "ADisplayMethod.h"
//#include "DisplayMethodNames.h"
//#include "DataSourceTypeMapping.h"
//#include "DMTypeMap.generated.h"
//
//UCLASS(BlueprintType)
//class UDMTypeMap : public UADisplayMethod
//{
//	GENERATED_BODY()
//
//public:
//    explicit UDMTypeMap()
//        : UADisplayMethod()
//    {
//    	method = DISPLAY_METHOD_TYPE_MAP;
//    }
//
//	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Type map")
//		// ReSharper disable once CppInconsistentNaming, need to map with the json
//		TArray<FDataSourceTypeMapping*> mapping;
//	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Type map")
//		// ReSharper disable once CppInconsistentNaming, need to map with the json
//		FString material;
//	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Type map")
//		// ReSharper disable once CppInconsistentNaming, need to map with the json
//		FString type_material;
//	UPROPERTY(BlueprintReadOnly, Category = "Config|Data model|Layer|Display method|Type map")
//		bool seabottom;
//};
