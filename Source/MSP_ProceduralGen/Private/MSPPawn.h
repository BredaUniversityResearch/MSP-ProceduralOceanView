#pragma once

#include "InputMappingContext.h"
#include "GameFramework/FloatingPawnMovement.h"

#include "MSPPawn.generated.h"

class UCameraComponent;
class USphereComponent;
class UMSPPawnCameraPresetComponent;

UCLASS(Blueprintable, BlueprintType)
class AMSPPawn : public APawn
{
	GENERATED_BODY()

	static const FName CollisionComponentName;
	static const FName MovementComponentName;
	static const FName CameraPresetComponentName;

public:
	explicit AMSPPawn(const FObjectInitializer& Initializer);

	virtual UPawnMovementComponent* GetMovementComponent() const override { return MovementComponent; }
	virtual void PawnClientRestart() override;

	virtual void BeginPlay() override;
	virtual void MoveForward(float Val);
	virtual void MoveRight(float Val);
	virtual void MoveUp_World(float Val);

	void ChangeMovementSpeed(float Val);

	UPROPERTY(EditDefaultsOnly)
	UMSPPawnCameraPresetComponent* CameraPresetComponent;

protected:

	UFUNCTION(BlueprintImplementableEvent)
	void PropogateMovementSpeedToBPUI(float MovementSpeed) const;

private:
	void ApplyMoveSpeed(float LinearScalar);

	UPROPERTY(EditDefaultsOnly);
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* CollisionComponent;
	UPROPERTY(EditDefaultsOnly)
	UFloatingPawnMovement* MovementComponent;

	UPROPERTY(VisibleAnywhere)
		float MoveSpeedScaleCounter{ 1.0f };
	UPROPERTY(EditAnywhere)
		float MoveSpeedScaleScale{ 24.0f };
	UPROPERTY(EditAnywhere)
		float MinMovementSpeed{ 1200.0f };
	UPROPERTY(EditAnywhere)
		float MaxMovementSpeed{ 12000.0f };
	UPROPERTY(EditAnywhere)
		float MinAcceleration{ 4000.0f };
	UPROPERTY(EditAnywhere)
		float MaxAcceleration{ 40000.0f };
	UPROPERTY(EditAnywhere)
		float MinDeceleration{ 8000.0f };
	UPROPERTY(EditAnywhere)
		float MaxDeceleration{ 80000.0f };
};
