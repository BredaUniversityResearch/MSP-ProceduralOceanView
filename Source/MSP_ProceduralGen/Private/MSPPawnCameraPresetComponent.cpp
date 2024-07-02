#include "MSPPawnCameraPresetComponent.h"
#include "Config/Config.h"
#include "Kismet/GameplayStatics.h"
#include "MSPProcGenActorSelectable.h"

void UMSPPawnCameraPresetComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMSPPawnCameraPresetComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UMSPPawnCameraPresetComponent::GoToCameraPreset(const int a_index) const
{
	const UConfig* config = UConfig::GetInstance();
	if (!config) return;
	FTransform transform;
	switch (a_index)
	{
		default:
		case 0:
			transform = GetCenterViewTransform(config->datamodel);
			break;
		case 1:
			transform = GetBirdEyeViewTransform(config->datamodel);
			break;
		case 2:
			transform = GetPointOfInterestTransform();
			break;
		case 3:
			transform = GetRandomViewTransform(config->datamodel);
			break;
	}
	SetPlayerLocationAndView(transform);
}

int UMSPPawnCameraPresetComponent::GetCameraPresetCount()
{
	return 4;
}

void UMSPPawnCameraPresetComponent::FocusObject(AMSPProcGenActorSelectable *a_selectedActorReference) const
{
	FVector targetLocation = a_selectedActorReference->GetActorLocation();
	TArray<UActorComponent*> components = a_selectedActorReference->GetComponentsByTag(UPrimitiveComponent::StaticClass(), FName("camera_target"));
	if (components.Num() > 0) {
		const UPrimitiveComponent* staticMeshComponent = Cast<UPrimitiveComponent>(components[0]);
		targetLocation = staticMeshComponent->GetComponentLocation();
	}

	auto transform = CalculateOrbitLookAtTransform(targetLocation, PointOfInterestDistance, PointOfInterestHorizontalAngle, PointOfInterestVerticalAngle);

	SetPlayerLocationAndView(transform);

}

//void UMSPPawnCameraPresetComponent::OnConfigDataLoaded(const UConfig* a_config) const
//{
//	GoToCameraPreset(0);
//}

void UMSPPawnCameraPresetComponent::SetPlayerLocationAndView(const FTransform& a_transform) const
{
	if (APawn* pawn = Cast<APawn>(GetOwner()))
	{
		pawn->SetActorTransform(FTransform(FQuat::Identity, a_transform.GetTranslation()), false, nullptr, ETeleportType::ResetPhysics);
		if (APlayerController* controller = Cast<APlayerController>(pawn->Controller))
		{
			controller->SetControlRotation(FRotator(a_transform.GetRotation()));
		}
	}
}

FTransform UMSPPawnCameraPresetComponent::GetCurrentTransform() const
{
	if (const APawn* pawn = Cast<APawn>(GetOwner())) {
		return pawn->GetActorTransform();
	}
	return {};
}

FTransform UMSPPawnCameraPresetComponent::GetCenterViewTransform(const UConfigData* a_configData) const
{
	if (!a_configData) return GetCurrentTransform();
	const FVector2D min = a_configData->GetCoordinate0AsWorldVector2D();
	const FVector2D max = a_configData->GetCoordinate1AsWorldVector2D();
	const FVector2D center = min + ((max - min) * 0.5);
	return (FTransform(FQuat::Identity, FVector(center, CenterViewHeight)));
}

FTransform UMSPPawnCameraPresetComponent::GetBirdEyeViewTransform(const UConfigData* a_configData) const
{
	if (!a_configData) return GetCurrentTransform();
	const FVector2D min = a_configData->GetCoordinate0AsWorldVector2D();
	const FVector2D max = a_configData->GetCoordinate1AsWorldVector2D();
	const FVector2D delta = max - min;
	const double cameraDistance = FMath::Min(delta.Length() * 0.5, BirdEyeViewDistance);
	const FVector lookAtPosition = FVector(min + (delta * 0.5), 0.0);
	return CalculateOrbitLookAtTransform(lookAtPosition, cameraDistance, BirdEyeViewHorizontalAngle, BirdEyeViewVerticalAngle);
}

FTransform UMSPPawnCameraPresetComponent::GetPointOfInterestTransform() const
{
	TArray<AActor*> targets;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("camera_target"), targets); // find by tag
	const AActor* target = nullptr;
	if (targets.Num() > 0) {
		target = targets[0];
	}
	if (target == nullptr) {
		target = FindFirstObject<AActor>(TEXT("Wind_Power_Turbine_C_0")); // find by name
	}
	if (target == nullptr) return GetCurrentTransform();

	FVector targetLocation = target->GetActorLocation();
	TArray<UActorComponent*> components = target->GetComponentsByTag(UPrimitiveComponent::StaticClass(), FName("camera_target"));
	if (components.Num() > 0) {
		const UPrimitiveComponent* staticMeshComponent = Cast<UPrimitiveComponent>(components[0]);
		targetLocation = staticMeshComponent->GetComponentLocation();
	}

	return CalculateOrbitLookAtTransform(targetLocation, PointOfInterestDistance, PointOfInterestHorizontalAngle, PointOfInterestVerticalAngle);
}

FTransform UMSPPawnCameraPresetComponent::GetRandomViewTransform(const UConfigData* a_configData) const
{
	if (!a_configData) return GetCurrentTransform();
	const FVector2D min = a_configData->GetCoordinate0AsWorldVector2D();
	const FVector2D max = a_configData->GetCoordinate1AsWorldVector2D();
	const FVector2D center = min + ((max - min) * 0.5);
	// random camera pos
	const FVector cameraPos(min.X + FMath::FRand() * (max.X - min.X), min.Y + FMath::FRand() * (max.Y - min.Y), RandomViewHeight);
	const FMatrix rotation = FRotationMatrix::MakeFromX(FVector(center.X, center.Y, 0.0f) - cameraPos);
	return FTransform(rotation.ToQuat(), cameraPos);
}

FTransform UMSPPawnCameraPresetComponent::CalculateOrbitLookAtTransform(const FVector& a_targetLocation, const double a_distance, const double a_horizontalCameraAngle, const double a_verticalCameraAngle) const
{
    // Convert angles from degrees to radians
    const float horizontalAngleRad = FMath::DegreesToRadians(a_horizontalCameraAngle);
    const float verticalAngleRad = FMath::DegreesToRadians(a_verticalCameraAngle);

    // Calculate camera position in world space
    const FVector cameraPosition = a_targetLocation - FVector(
        a_distance * FMath::Cos(verticalAngleRad) * FMath::Sin(horizontalAngleRad),
        a_distance * FMath::Cos(verticalAngleRad) * FMath::Cos(horizontalAngleRad),
        a_distance * FMath::Sin(verticalAngleRad)
    );

    // Calculate rotation for the camera to look at the target
    const FRotator cameraRotation = FRotationMatrix::MakeFromX(a_targetLocation - cameraPosition).Rotator();

    // Create a transform using the calculated position and rotation
    return FTransform(cameraRotation, cameraPosition);
}
