#pragma once

UENUM(BlueprintType)
enum class EDataPoolDataSourceLoadedType: uint8
{
	NotLoaded = 0 UMETA(DisplayName = "Unloaded, no default values available"),
	Default = 1 UMETA(DisplayName = "Initialized with default values"),
	LocalFile = 2 UMETA(DisplayName = "Loaded from local file"),
	Json = 3 UMETA(DisplayName = "Loaded from Json")
};
