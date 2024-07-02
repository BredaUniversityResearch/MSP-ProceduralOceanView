#include "MSPProcGenGameMode.h"

#include "GenerationSettings.h"
#include "MSPProcGenGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "MSP_ProceduralGen/MSP_ProceduralGen.h"
#include "ProceduralRulePack/ProceduralRulePackManager.h"
#include "Config/MSPConfigFilePathResolver.h"
#include "Config/Helpers/MSPConfigFileLoadHelper.h"
#include "DataPool/DataPool.h"
#include "GeoProjection/GeoCoordinateSystemTransformation.h"

AMSPProcGenGameMode::AMSPProcGenGameMode(const FObjectInitializer& Initializer)
{
	PRPManager = CreateDefaultSubobject<UProceduralRulePackManager>(FName("ProceduralRulePackManager"));
}

void AMSPProcGenGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UMSPProcGenGameInstance* gameInstance = GetGameInstance<UMSPProcGenGameInstance>();
	if (gameInstance != nullptr)
	{
		TSoftObjectPtr<UWorld> targetLevel;
		const FViewport* viewport = GetWorld()->GetGameViewport()->Viewport;
		// @note (MH): enable this code to enable a VR link mode from the VR Preview viewport in the editor
		if (gameInstance->GetGenerationSettings()->IsVR/* || (viewport && viewport->IsStereoRenderingAllowed())*/)
		{
			targetLevel = VRSubLevel;
			DefaultPawnClass = VRPawnClass;
		}
		else
		{
			targetLevel = StandAloneSubLevel;
		}

		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(), targetLevel, true, true, FLatentActionInfo());

		PRPManager->SetTargetDataPool(gameInstance->GetDataPool());

		FMSPConfigFileLoadHelper configFileLoadHelper{};
		auto filePath = gameInstance->GetGenerationSettings()->JsonFilePath;
		LoadedConfig = configFileLoadHelper.Load(gameInstance->GetGenerationSettings()->JsonFilePath);

		if (LoadedConfig == nullptr)
		{
			UE_LOG(LogMspProceduralGen, Error, TEXT("Failed to load config file from %s"), *gameInstance->GetGenerationSettings()->JsonFilePath);
		}

		SimulationProjection.SimulatedBounds.Min = LoadedConfig->datamodel->GetCoordinate0AsWorldVector2D();
		SimulationProjection.SimulatedBounds.Max = LoadedConfig->datamodel->GetCoordinate1AsWorldVector2D();
		SimulationProjection.WorldOffset = LoadedConfig->datamodel->GetWorldOffset();

		LoadedConfigToLatLon = UGeoCoordinateSystemTransformation::TryCreateFromProjWKT(LoadedConfig->datamodel->projection, TEXT("+proj=longlat +datum=WGS84 +no_defs +type=crs"));

		gameInstance->GetDataPool().Setup(FMSPConfigFilePathResolver(gameInstance->GetGenerationSettings()->JsonFilePath), LoadedConfig);
	}
	else
	{
		UE_LOG(LogMspProceduralGen, Error, TEXT("Game instance of ProcGenGameInstance could not be found."));
	}
}

void AMSPProcGenGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UMSPProcGenGameInstance* gameInstance = GetGameInstance<UMSPProcGenGameInstance>();
	if (gameInstance != nullptr)
	{
		gameInstance->ResetDataPool();
	}
}

UConfig* AMSPProcGenGameMode::GetLoadedConfig() const
{
	return LoadedConfig;
}
