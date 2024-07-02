#include "EnvironmentStateController.h"

#include "EnvironmentStateSettings.h"
#include "HeightFogStateController.h"
#include "MSPProcGenGameMode.h"
#include "SkyCreatorStateController.h"
#include "WaterBodyStateController.h"
#include "GeoProjection/GeoCoordinateHelper.h"
#include "MSP_ProceduralGen/MSP_ProceduralGen.h"
#include "WeatherAPI/OpenMeteoAPIRequest.h"

AEnvironmentStateController::AEnvironmentStateController(const FObjectInitializer&)
{
	CurrentSettings = CreateDefaultSubobject<UEnvironmentStateSettings>("DefaultEnvironmentSettings", false);
	SkyCreatorComponent = CreateDefaultSubobject<USkyCreatorStateController>("SkyCreatorController", false);
	HeightFogController = CreateDefaultSubobject<UHeightFogStateController>("HeightFogController", false);

	PrimaryActorTick.bCanEverTick = true;
}

void AEnvironmentStateController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	SecondsSinceLastDiscovery += DeltaSeconds;
	bool canFindTargets = false;
	if (SecondsSinceLastDiscovery > ActorDiscoveryIntervalSeconds)
	{
		SecondsSinceLastDiscovery = 0.0f; //Don't care about spill / inaccuracy
		canFindTargets = true;
	}

	//Since GC can actually clean up some stuff because of UI changes... We need to check the elements for validity and then rediscover if we lose one or more.
	int32 oldControllerInstancesNum = ControllerInstances.Num();
	int32 newControllerInstancesNum = ControllerInstances.RemoveAllSwap([](const UObject* Elem) { return !IsValid(Elem); });
	if (oldControllerInstancesNum != newControllerInstancesNum)
	{
		DiscoverComponents();
		canFindTargets = false;
		SecondsSinceLastDiscovery = 0.0f;
	}

	for (auto* comp : ControllerInstances)
	{
		if (UpdateInEditor || canFindTargets)
		{
			IEnvironmentStateControllerComponent::Execute_FindTargetComponents(comp);
		}
		IEnvironmentStateControllerComponent::Execute_UpdateTargetSettings(comp, DeltaSeconds, CurrentSettings);
	}
}

void AEnvironmentStateController::ApplyLatestRealWorldConditions()
{
	ApplyWeatherConditions(LatestCurrentWeatherConditions);
	ApplyMarineConditions(LatestCurrentMarineConditions);
}

void AEnvironmentStateController::BeginPlay()
{
	Super::BeginPlay();

	double currentHour = FDateTime::Now().GetTimeOfDay().GetTotalHours();
	CurrentSettings->EnvironmentState.TimeOfDay = currentHour / 24.0;

	DiscoverComponents();

	AsyncUpdateWeatherConfitionsFromRealWorldData();
}

void AEnvironmentStateController::DiscoverComponents()
{
	TArray<UActorComponent*> components = GetComponentsByInterface(UEnvironmentStateControllerComponent::StaticClass());
	for (UActorComponent* comp : components)
	{
		UObject* targetComponent = Cast<UObject>(comp);
		if (targetComponent != nullptr && !ControllerInstances.Contains(targetComponent))
		{
			ControllerInstances.Emplace(targetComponent);
			IEnvironmentStateControllerComponent::Execute_FindTargetComponents(comp);
		}
	}
}

void AEnvironmentStateController::AsyncUpdateWeatherConfitionsFromRealWorldData()
{
	AMSPProcGenGameMode* gameMode = GetWorld()->GetAuthGameMode<AMSPProcGenGameMode>();
	if (gameMode != nullptr)
	{
		FLatLonCoordinate coord = UGeoCoordinateHelper::TransformWorldPositionToLatLon(GetWorld(), FVector(gameMode->SimulationProjection.SimulatedBounds.GetCenter(), 0.0));

		TWeakObjectPtr<AEnvironmentStateController> targetController = this;

		FOpenMeteoWeatherInfoReceived receivedCallback;
		receivedCallback.BindLambda([targetController](bool Success, const FOpenMeteoWeatherResult& Result) {
			if (Success)
			{
				if (AEnvironmentStateController* controller = targetController.Get())
				{
					controller->LatestCurrentWeatherConditions = Result;
					controller->ApplyWeatherConditions(Result);
				}
			}
			else
			{
				UE_LOG(LogMspProceduralGen, Warning, TEXT("Environment state controller request for weather info failed."));
			}
		});
		FOpenMeteoAPI::RequestWeatherInfo(coord, MoveTemp(receivedCallback));

		FOpenMeteoMarineInfoReceived marineInfoReceivedCallback;
		marineInfoReceivedCallback.BindLambda([targetController](bool Success, const FOpenMeteoMarineResult& Result) {
			if (Success)
			{
				if (AEnvironmentStateController* controller = targetController.Get())
				{
					controller->LatestCurrentMarineConditions = Result;
					controller->ApplyMarineConditions(Result);
				}
			}
			else
			{
				UE_LOG(LogMspProceduralGen, Warning, TEXT("Environment state controller request for marine state info failed."));
			}
		});
		FOpenMeteoAPI::RequestMarineInfo(coord, MoveTemp(marineInfoReceivedCallback));
	}
}

void AEnvironmentStateController::ApplyWeatherConditions(const FOpenMeteoWeatherResult& ResultToApply)
{
	CurrentSettings->EnvironmentState.Precipitation = FMath::GetRangePct(0.0f, 100.0f, ResultToApply.Precipitation);
	CurrentSettings->EnvironmentState.CloudCover = ResultToApply.CloudCover / 100.0f;
	CurrentSettings->EnvironmentState.WindSpeed = ResultToApply.WindSpeed_10m * MetersToKnots;
	CurrentSettings->EnvironmentState.WindDirection = ResultToApply.WindDirection_10m;
}

void AEnvironmentStateController::ApplyMarineConditions(const FOpenMeteoMarineResult& ResultToApply)
{
	CurrentSettings->EnvironmentState.WaveHeight = FMath::GetRangePct(0.0f, 10.0f, ResultToApply.WaveHeight);
}
