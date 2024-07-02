#pragma once

#include "CoreMinimal.h"
#include "WeatherAPI/OpenMeteoAPIRequest.h"
#include "EnvironmentStateController.generated.h"

class UEnvironmentStateSettings;
class UEnvironmentStateControllerComponent;

UCLASS()
class MSP_PROCEDURALGEN_API AEnvironmentStateController : public AActor
{
	GENERATED_BODY()

	static constexpr float MetersToKnots = 1.94384449f;
public:
	explicit AEnvironmentStateController(const FObjectInitializer&);

	virtual void Tick(float DeltaSeconds) override;
	
	virtual bool ShouldTickIfViewportsOnly() const override
	{
		return UpdateInEditor;
	}

	UFUNCTION(BlueprintCallable)
	UEnvironmentStateSettings* GetEnvironmentStateSettings() { return CurrentSettings; }
	UFUNCTION(BlueprintCallable)
	void ApplyLatestRealWorldConditions();

protected:
	virtual void BeginPlay() override;

private:
	void DiscoverComponents();
	void AsyncUpdateWeatherConfitionsFromRealWorldData();

	void ApplyWeatherConditions(const FOpenMeteoWeatherResult& ResultToApply);
	void ApplyMarineConditions(const FOpenMeteoMarineResult& ResultToApply);

	UPROPERTY(EditAnywhere)
	class USkyCreatorStateController* SkyCreatorComponent{nullptr};

	UPROPERTY(EditAnywhere)
	class UHeightFogStateController* HeightFogController{nullptr};
	
	UPROPERTY(VisibleAnywhere, Transient)
	TArray<UObject*> ControllerInstances{};

	UPROPERTY(Instanced, EditAnywhere, Meta = (ShowOnlyInnerProperties))
	class UEnvironmentStateSettings* CurrentSettings;

	UPROPERTY(EditAnywhere)
	bool UpdateInEditor{ true };

	UPROPERTY(EditAnywhere)
	float ActorDiscoveryIntervalSeconds = 1.0f;
	float SecondsSinceLastDiscovery = 0.0f;

	UPROPERTY(Transient)
	FOpenMeteoWeatherResult LatestCurrentWeatherConditions;
	UPROPERTY(Transient)
	FOpenMeteoMarineResult LatestCurrentMarineConditions;

};
