#pragma once

#include "MSPPawnCameraPresetComponent.generated.h"

class UConfig;
class UConfigData;
class AMSPProcGenActorSelectable;

UCLASS()
class UMSPPawnCameraPresetComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void GoToCameraPreset(int a_index) const;
	static int GetCameraPresetCount();

	void FocusObject( AMSPProcGenActorSelectable* a_selectedActorReference ) const;

private:
	//void OnConfigDataLoaded(const UConfig* a_config) const;
	void SetPlayerLocationAndView(const FTransform& a_transform) const;
	FTransform GetCurrentTransform() const;
	FTransform GetCenterViewTransform(const UConfigData* a_configData) const;
	FTransform GetBirdEyeViewTransform(const UConfigData* a_configData) const;
	FTransform GetPointOfInterestTransform() const;
	FTransform GetRandomViewTransform(const UConfigData* a_configData) const;
	FTransform CalculateOrbitLookAtTransform(const FVector& a_targetLocation, const double a_distance, const double a_horizontalCameraAngle, const double a_verticalCameraAngle) const;

	UPROPERTY(EditAnywhere)
	double CenterViewHeight = 8000.0;

	UPROPERTY(EditAnywhere)
	double BirdEyeViewHorizontalAngle = 32.0;
	UPROPERTY(EditAnywhere)
	double BirdEyeViewDistance = 50000.0;
	UPROPERTY(EditAnywhere)
	double BirdEyeViewVerticalAngle = -15.0;

	UPROPERTY(EditAnywhere)
	double PointOfInterestDistance = 1500.0;
	UPROPERTY(EditAnywhere)
	double PointOfInterestHorizontalAngle = 32.0;
	UPROPERTY(EditAnywhere)
	double PointOfInterestVerticalAngle = 85.0;

	UPROPERTY(EditAnywhere)
	double RandomViewHeight = 100.0;
};