#include "MSPPawn.h"

#include "EnhancedInputSubsystems.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "MSPPawnCameraPresetComponent.h"

const FName AMSPPawn::CollisionComponentName = FName("PawnSphereCollider");
const FName AMSPPawn::MovementComponentName = FName("PawnMovement");
const FName AMSPPawn::CameraPresetComponentName = FName("CameraPresets");

AMSPPawn::AMSPPawn(const FObjectInitializer& Initializer)
	: Super(Initializer)
{
	SetCanBeDamaged(true);

	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	NetPriority = 3.0f;

	BaseEyeHeight = 0.0f;
	bCollideWhenPlacing = false;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(CollisionComponentName);
	CollisionComponent->InitSphereRadius(35.0f);
	CollisionComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	CollisionComponent->CanCharacterStepUpOn = ECB_No;
	CollisionComponent->SetShouldUpdatePhysicsVolume(true);
	CollisionComponent->SetCanEverAffectNavigation(false);
	CollisionComponent->bDynamicObstacle = true;

	RootComponent = CollisionComponent;

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(MovementComponentName);
	MovementComponent->SetUpdatedComponent(CollisionComponent);

	CameraPresetComponent = CreateDefaultSubobject<UMSPPawnCameraPresetComponent>(CameraPresetComponentName);
}

void AMSPPawn::PawnClientRestart()
{
	Super::PawnClientRestart();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (!InputMapping.IsNull())
			{
				Subsystem->ClearAllMappings();
				Subsystem->AddMappingContext(InputMapping.LoadSynchronous(), InputPriority);
			}
		}
	}
}

void AMSPPawn::BeginPlay()
{
	Super::BeginPlay();
	ApplyMoveSpeed( MoveSpeedScaleCounter / MoveSpeedScaleScale );
}

void AMSPPawn::MoveForward(float Val)
{
	if (Val != 0.0f && Controller != nullptr)
	{
		FRotator rotation = Controller->GetControlRotation();
		AddMovementInput(FRotationMatrix(rotation).GetScaledAxis(EAxis::X), Val);
	}
}

void AMSPPawn::MoveRight(float Val)
{
	if (Val != 0.0f && Controller != nullptr)
	{
		FRotator rotation = Controller->GetControlRotation();
		AddMovementInput(FRotationMatrix(rotation).GetScaledAxis(EAxis::Y), Val);
	}
}

void AMSPPawn::MoveUp_World(float Val)
{
	if (Val != 0.0f)
	{
		AddMovementInput(FVector::UpVector, Val);
	}
}

void AMSPPawn::ChangeMovementSpeed(float Val)
{
	MoveSpeedScaleCounter = FMath::Clamp(MoveSpeedScaleCounter + Val, 0.0f, MoveSpeedScaleScale);
	ApplyMoveSpeed(MoveSpeedScaleCounter / MoveSpeedScaleScale);
}

void AMSPPawn::ApplyMoveSpeed(float LinearScalar)
{
	PropogateMovementSpeedToBPUI(LinearScalar);

	float exponentLerp = FMath::Pow(LinearScalar, 2.0f);
	MovementComponent->MaxSpeed = FMath::Lerp(MinMovementSpeed, MaxMovementSpeed, exponentLerp);
	MovementComponent->Acceleration = FMath::Lerp(MinAcceleration, MaxAcceleration, exponentLerp);
	MovementComponent->Deceleration = FMath::Lerp(MinDeceleration, MaxDeceleration, exponentLerp);
}
