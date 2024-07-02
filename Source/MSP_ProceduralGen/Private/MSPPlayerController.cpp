#include "MSPPlayerController.h"
#include "MSPPawn.h"
#include "EnhancedInputComponent.h"
#include "MotionControllerComponent.h"
#include "MSPPawnCameraPresetComponent.h"
#include "MSPPlayerCameraController.h"
#include "MSPProcGenActorNiagaraSelectionComponent.h"
#include "MSPProcGenActorSelectable.h"
#include "Components/WidgetInteractionComponent.h"
#include "ProceduralRulePack/ProceduralRulePackActor.h"
#include "MSPProcGenGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "ProceduralRulePack/ProceduralRulePackManager.h"

namespace
{
	void GetRootPrimitiveComponents(const AActor *a_actor, TArray<UPrimitiveComponent *> &a_rootPrimitiveComponents, bool a_topLevel = true)
	{
		const AActor *target = a_actor;
		if (a_topLevel)
		{ // top level is requested so make sure we are at the top-level
			while (const UChildActorComponent *parentActorComponent = target->GetParentComponent())
			{
				AActor *parentActor = parentActorComponent->GetOwner();
				if (!parentActor)
				{
					break;
				}
				if (const AMSPProcGenActorSelectable *parentActorSelectable = Cast<AMSPProcGenActorSelectable>(parentActor))
				{
					target = parentActorSelectable;
				}
			}
		}

		USceneComponent *rootComponent = target->GetRootComponent();
		if (!rootComponent)
		{
			return;
		}
		TArray<USceneComponent *> level;
		level.Add(rootComponent);
		while (!level.IsEmpty())
		{
			TArray<USceneComponent *> nextLevel;
			for (USceneComponent *currentComponent : level)
			{ // traverse all leafs on this level
				if (UPrimitiveComponent *primitiveComp = Cast<UPrimitiveComponent>(currentComponent))
				{
					a_rootPrimitiveComponents.Add(primitiveComp);
				}
				if (const UChildActorComponent *childActorComponent = Cast<UChildActorComponent>(currentComponent))
				{
					if (const AActor *childActor = childActorComponent->GetChildActor())
					{
						TArray<UPrimitiveComponent *> childRootPrimitiveComponents;
						GetRootPrimitiveComponents(childActor, childRootPrimitiveComponents, false);
						a_rootPrimitiveComponents.Append(childRootPrimitiveComponents);
					}
				}
				// collect all children for next iteration, if any
				TArray<USceneComponent *> children;
				currentComponent->GetChildrenComponents(true, children);
				for (USceneComponent *child : children)
				{
					nextLevel.Add(child);
				}
			}
			if (a_rootPrimitiveComponents.Num() > 0)
			{ // found all primitive components on this level
				break;
			}
			level = nextLevel;
		}
	}

	void SetCustomDepthRenderEnabled(const AMSPProcGenActorSelectable *a_actor, const bool a_enabled = true)
	{
		TArray<UPrimitiveComponent *> primitiveRootComponents;
		GetRootPrimitiveComponents(a_actor, primitiveRootComponents);
		for (UPrimitiveComponent *primitiveComponent : primitiveRootComponents)
		{
			if (a_enabled)
			{
				primitiveComponent->SetRenderCustomDepth(true);
				primitiveComponent->SetCustomDepthStencilValue(128);
				continue;
			}
			primitiveComponent->SetRenderCustomDepth(false);
		}
	}

	UMotionControllerComponent *GetMotionControllerComponent(const AMSPPawn *a_pawn, const bool a_isLeftMotionController)
	{
		TArray<UMotionControllerComponent *> components;
		a_pawn->GetComponents<UMotionControllerComponent>(components);
		UMotionControllerComponent *targetMotionControllerComponent = nullptr;
		for (UMotionControllerComponent *motionControllerComponent : components)
		{
			if (a_isLeftMotionController ? motionControllerComponent->GetName() == "MotionControllerLeftAim" : motionControllerComponent->GetName() == "MotionControllerRightAim")
			{
				targetMotionControllerComponent = motionControllerComponent;
			}
		}
		return targetMotionControllerComponent;
	}

	UWidgetInteractionComponent *GetWidgetInteractionComponent(const AMSPPawn *a_pawn, const bool a_isLeftMotionController)
	{
		TArray<UWidgetInteractionComponent *> components;
		a_pawn->GetComponents<UWidgetInteractionComponent>(components);
		UWidgetInteractionComponent *widgetInteractionComponent = nullptr;
		for (UWidgetInteractionComponent *interactionComponent : components)
		{
			if (a_isLeftMotionController ? interactionComponent->GetName() == "WidgetInteractionLeft" : interactionComponent->GetName() == "WidgetInteractionRight")
			{
				widgetInteractionComponent = interactionComponent;
			}
		}
		return widgetInteractionComponent;
	}
}

void AMSPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto prpManager = Cast<AMSPProcGenGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->PRPManager;
	prpManager->OnDisplayModeToggled.AddDynamic(this, &AMSPPlayerController::OnDisplayModeToggled);

}

void AMSPPlayerController::OnDisplayModeToggled(FName a_displayModeName, bool a_currentlyEnabled)
{
	// chekc if selected is still valid

	if (!a_currentlyEnabled)
	{
		if (auto* actor = m_selectedActorReference.Get())
		{
			if (auto* prpActor = Cast<AProceduralRulePackActor>(actor))
			{
				if (prpActor->UIDisplayName == a_displayModeName)
				{
					SetSelectedActor(nullptr);
				}
			}
		}
		else{
			SetSelectedActor(nullptr);
		}
	}
}

void AMSPPlayerController::Select(
	const AMSPPlayerController &a_controller,
	FGetLineTranceStartAndEnd a_getLineTraceStartAndEndDelegate,
	const bool a_debugTrace,
	const float a_traceRadius)
{
	checkf(a_getLineTraceStartAndEndDelegate.IsBound(), TEXT("a_getLineTraceStartAndEndDelegate is not bound!"));
	if (const AMSPProcGenActorSelectable *currentSelectedActor = m_selectedActorReference.Get())
	{
		SetCustomDepthRenderEnabled(currentSelectedActor, false);
	}


	FVector lineTraceStart, lineTraceEnd;
	if (!a_getLineTraceStartAndEndDelegate.Execute(lineTraceStart, lineTraceEnd))
	{
		return;
	}

	// draw collision area for 3 sec, debugging purposes
	if (a_debugTrace)
	{
		DrawDebugCylinder(a_controller.GetWorld(), lineTraceStart, lineTraceEnd, a_traceRadius, 12, FColor::Green, false, 3.0f, 0, 1.0f);
	}

	// Set the half height based on the distance between start and end points
	const float halfHeight = static_cast<float>(FVector::Dist(lineTraceStart, lineTraceEnd)) * 0.5f;

	FCollisionShape traceShape = FCollisionShape::MakeCapsule(a_traceRadius, halfHeight);

	TArray<FHitResult> sweepResult;
	const bool bHit = a_controller.GetWorld()->SweepMultiByChannel(
		sweepResult,
		lineTraceStart,
		lineTraceEnd,
		FQuat::FindBetween(FVector::UpVector, (lineTraceEnd - lineTraceStart).GetSafeNormal()), // rotation flat along the line
		ECC_GameTraceChannel3,
		traceShape,
		FCollisionQueryParams::DefaultQueryParam);
	if (!bHit)
	{
		if (m_selectedActorReference != nullptr)
		{
			SetSelectedActor(nullptr);
		}
		return;
	}

	AMSPProcGenActorSelectable* actorToSelect = nullptr;
	for (const FHitResult& hitResult : sweepResult)
	{
		AMSPProcGenActorSelectable *actor = Cast<AMSPProcGenActorSelectable>(hitResult.GetActor());
		if (actor != nullptr)
		{
			if (UMSPProcGenActorNiagaraSelectionComponent* niagaraSelection = actor->GetComponentByClass<UMSPProcGenActorNiagaraSelectionComponent>())
			{
				if (niagaraSelection->PerformSubSelection(traceShape, lineTraceStart, lineTraceEnd))
				{
					actorToSelect = actor;
					break;
				}
			}
			else
			{
				actorToSelect = actor;
				break;
			}
		}
	}

	//m_selectedActorReference = actorToSelect;
	if (actorToSelect != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Object selected: %s"), *actorToSelect->GetName());
	}
	SetSelectedActor(actorToSelect);
}

void AMSPPlayerController::SetSelectedActor(AMSPProcGenActorSelectable *a_selectedActorReference)
{
	if (const AMSPProcGenActorSelectable* selectable = m_selectedActorReference.Get())
	{
		SetCustomDepthRenderEnabled(selectable, false);
	}

	if (m_selectedActorReference != nullptr)
	{
		m_selectedActorReference->OnDeselected();
	}

	m_selectedActorReference = a_selectedActorReference;

	if (a_selectedActorReference != nullptr)
	{
		SetCustomDepthRenderEnabled(a_selectedActorReference, true);
	}

	OnPlayerSelectObject.Broadcast(a_selectedActorReference);

	auto* prpActor = Cast<AProceduralRulePackActor>(a_selectedActorReference);
	if (prpActor)
	{
		prpActor->OnSelected();
	}

}

void AMSPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent *Input = Cast<UEnhancedInputComponent>(InputComponent);
	Input->BindAction(InputActionMove, ETriggerEvent::Triggered, this, &AMSPPlayerController::OnPlayerMove);
	Input->BindAction(InputActionMoveUpDown, ETriggerEvent::Triggered, this, &AMSPPlayerController::OnPlayerMoveUpDown);
	Input->BindAction(InputActionMouseLook, ETriggerEvent::Triggered, this, &AMSPPlayerController::OnInputMouseLook);
	Input->BindAction(InputActionLookHold, ETriggerEvent::Triggered, this, &AMSPPlayerController::OnInputLookHold);
	Input->BindAction(InputActionSelect, ETriggerEvent::Triggered, this, &AMSPPlayerController::OnInputSelect);
	Input->BindAction(InputActionAdjustMoveSpeed, ETriggerEvent::Triggered, this, &AMSPPlayerController::OnInputAdjustMoveSpeed);
	Input->BindAction(InputActionNextCameraPreset, ETriggerEvent::Triggered, this, &AMSPPlayerController::OnInputNextCameraPreset);
	Input->BindAction(InputActionDoubleClick, ETriggerEvent::Triggered, this, &AMSPPlayerController::OnInputDoubleClick);
	Input->BindAction(InputActionCamera1, ETriggerEvent::Triggered, this, &AMSPPlayerController::OnInputCamera1);
	Input->BindAction(InputActionCamera2, ETriggerEvent::Triggered, this, &AMSPPlayerController::OnInputCamera2);
	Input->BindAction(InputActionCamera3, ETriggerEvent::Triggered, this, &AMSPPlayerController::OnInputCamera3);
	Input->BindAction(InputActionCamera4, ETriggerEvent::Triggered, this, &AMSPPlayerController::OnInputCamera4);
	Input->BindAction(InputActionFocusObject, ETriggerEvent::Triggered, this, &AMSPPlayerController::OnInputFocusObject);

	// VR specific
	Input->BindAction(InputActionVRSelectLeft, ETriggerEvent::Triggered, this, &AMSPPlayerController::OnInputVRSelectLeft);
	Input->BindAction(InputActionVRSelectRight, ETriggerEvent::Triggered, this, &AMSPPlayerController::OnInputVRSelectRight);
	Input->BindAction(InputActionVRAdjustMoveSpeed, ETriggerEvent::Triggered, this, &AMSPPlayerController::OnInputVRAdjustMoveSpeed);
	Input->BindAction(InputActionVRGripLeft, ETriggerEvent::Triggered, this, &AMSPPlayerController::OnInputVRGripLeft);
	Input->BindAction(InputActionVRGripRight, ETriggerEvent::Triggered, this, &AMSPPlayerController::OnInputVRGripRight);

	SetShowMouseCursor(true);
}

void AMSPPlayerController::OnPlayerMove(const FInputActionValue &a_axisValue)
{
	FVector2D axisInput = a_axisValue.Get<FVector2D>();

	if (AMSPPawn *pawn = GetPawn<AMSPPawn>())
	{
		pawn->MoveForward(static_cast<float>(axisInput.Y));
		pawn->MoveRight(static_cast<float>(axisInput.X));
	}
}

void AMSPPlayerController::OnPlayerMoveUpDown(const FInputActionValue &a_inputActionValue)
{
	if (AMSPPawn *pawn = GetPawn<AMSPPawn>())
	{
		pawn->MoveUp_World(a_inputActionValue.Get<float>());
	}
}

void AMSPPlayerController::OnInputMouseLook(const FInputActionValue &a_inputActionValue)
{
	if (AMSPPawn *pawn = GetPawn<AMSPPawn>(); pawn && m_shouldProcessMouseMove)
	{
		const FVector2D mouseInput = a_inputActionValue.Get<FVector2D>();
		pawn->AddControllerYawInput(static_cast<float>(mouseInput.X));
		pawn->AddControllerPitchInput(static_cast<float>(-mouseInput.Y));

		int32 sizeX, sizeY;
		GetViewportSize(sizeX, sizeY);
		SetMouseLocation(sizeX / 2, sizeY / 2);
	}
}

void AMSPPlayerController::OnInputLookHold(const FInputActionValue &a_inputActionValue)
{
	if (const float val = a_inputActionValue.Get<float>(); val < 0.5f)
	{
		m_shouldProcessMouseMove = false;
		SetMouseLocation(static_cast<int>(m_mouseDownPosition.X), static_cast<int>(m_mouseDownPosition.Y));
		PlayerLookHoldFinished.Broadcast();
	}
	else
	{
		m_shouldProcessMouseMove = true;
		GetMousePosition(m_mouseDownPosition.X, m_mouseDownPosition.Y);
		PlayerLookHoldStarted.Broadcast();
	}
	SetShowMouseCursor(!m_shouldProcessMouseMove);
}

void AMSPPlayerController::OnInputSelect(const FInputActionValue &a_inputActionValue)
{
	float screenX {};
	float screenY{};
	if (!GetMousePosition(screenX, screenY))
	{
		return;
		//verify(false);
	}

	FGetLineTranceStartAndEnd delegate;
	delegate.BindLambda(
		[this, screenX, screenY](FVector &a_lineTraceStart, FVector &a_lineTraceEnd)
		{
			FVector worldLoc, worldDir;
			if (!DeprojectScreenPositionToWorld(screenX, screenY, worldLoc, worldDir))
			{
				return false;
			}

			a_lineTraceStart = worldLoc;
			a_lineTraceEnd = worldLoc + worldDir * 200000;

			return true;
		});

	Select(*this, delegate, m_DebugSelectionTrace, m_SelectionTraceRadius);
}

void AMSPPlayerController::OnInputAdjustMoveSpeed(const FInputActionValue &a_inputActionValue)
{
	const float val = a_inputActionValue.Get<float>();
	if ( val != 0.0f && m_shouldProcessMouseMove)
	{
		AMSPPawn *pawn = GetPawn<AMSPPawn>();
		if (pawn != nullptr)
		{
			pawn->ChangeMovementSpeed(val);
		}
	}
}

void AMSPPlayerController::OnInputVRAdjustMoveSpeed(const FInputActionValue &a_axisValue)
{
	FVector2D axisInput = a_axisValue.Get<FVector2D>();
	if (AMSPPawn *pawn = GetPawn<AMSPPawn>())
	{
		pawn->ChangeMovementSpeed(axisInput.X * 4.0);
	}
}

void AMSPPlayerController::OnInputNextCameraPreset(const FInputActionValue &a_inputActionValue)
{
	if (const AMSPPawn *pawn = GetPawn<AMSPPawn>())
	{
		m_currentCameraPreset = (m_currentCameraPreset + 1) % pawn->CameraPresetComponent->GetCameraPresetCount();
		pawn->CameraPresetComponent->GoToCameraPreset(m_currentCameraPreset);
	}
}

void AMSPPlayerController::OnInputDoubleClick(const FInputActionValue &a_inputActionValue)
{
	const float val = a_inputActionValue.Get<float>();
	if (val == 0.0f)
	{					// released
		m_clickCount++; // another click!
		// fail-safe: do not count above 2.
		if (m_clickCount > 2)
		{
			m_clickCount = 2;
		}
	}
	if (m_clickCount == 1)
	{
		// set the timer for resetting the click count. If there is a second click within this time, this is a "double click".
		GetWorld()->GetTimerManager().SetTimer(m_resetClickCountTimerHandle, FTimerDelegate::CreateWeakLambda(this, [this]()
																											  {
			m_clickCount = 0;
			GetWorld()->GetTimerManager().ClearTimer(m_resetClickCountTimerHandle); }),
											   m_resetClickCountDelaySec, false);
	}
	if (m_clickCount == 2)
	{ // double click
		// reset click count for a new double click.
		m_clickCount = 0;
		// stops the timer
		GetWorld()->GetTimerManager().ClearTimer(m_resetClickCountTimerHandle);

		if (auto* actor = m_selectedActorReference.Get())
		{
			UE_LOG(LogTemp, Log, TEXT("Double click on selected: %s"), *m_selectedActorReference->GetName());

			actor->OnDoubleClickedEvent();
		}
	}
}

void AMSPPlayerController::OnInputCamera1(const FInputActionValue &a_inputActionValue)
{
	if (const AMSPPawn *pawn = GetPawn<AMSPPawn>())
	{
		pawn->CameraPresetComponent->GoToCameraPreset(0);
	}
}

void AMSPPlayerController::OnInputCamera2(const FInputActionValue &a_inputActionValue)
{
	if (const AMSPPawn *pawn = GetPawn<AMSPPawn>())
	{
		pawn->CameraPresetComponent->GoToCameraPreset(1);
	}
}

void AMSPPlayerController::OnInputCamera3(const FInputActionValue &a_inputActionValue)
{
	if (const AMSPPawn *pawn = GetPawn<AMSPPawn>())
	{
		pawn->CameraPresetComponent->GoToCameraPreset(2);
	}
}

void AMSPPlayerController::OnInputCamera4(const FInputActionValue &a_inputActionValue)
{
	if (const AMSPPawn *pawn = GetPawn<AMSPPawn>())
	{
		pawn->CameraPresetComponent->GoToCameraPreset(3);
	}
}

void AMSPPlayerController::OnInputFocusObject(const FInputActionValue &a_inputActionValue)
{
	if (const AMSPPawn *pawn = GetPawn<AMSPPawn>())
	{
		if (m_selectedActorReference != nullptr)
		{
			pawn->CameraPresetComponent->FocusObject(m_selectedActorReference.Get());
		}
	}
}

void AMSPPlayerController::OnInputVRSelectLeft(const FInputActionValue &a_inputActionValue)
{
	OnInputVRSelect(a_inputActionValue, true);
}

void AMSPPlayerController::OnInputVRSelectRight(const FInputActionValue &a_inputActionValue)
{
	OnInputVRSelect(a_inputActionValue, false);
}

void AMSPPlayerController::OnInputVRSelect(const FInputActionValue &a_inputActionValue, const bool a_isLeftMotionController)
{
	FGetLineTranceStartAndEnd delegate;
	delegate.BindLambda(
	[this, a_isLeftMotionController](FVector &a_lineTraceStart, FVector &a_lineTraceEnd)
	{
	    const AMSPPawn* pawn = GetPawn<AMSPPawn>();
		if (!pawn) return false;
		const UMotionControllerComponent* targetMotionControllerComponent(GetMotionControllerComponent(pawn, a_isLeftMotionController));
		if (!targetMotionControllerComponent) return false;
    	const UWidgetInteractionComponent* widgetInteractionComponent(GetWidgetInteractionComponent(pawn, a_isLeftMotionController));
		if (!widgetInteractionComponent) return false;

		FVector2d screenLoc;
		if (!ProjectWorldLocationToScreen(targetMotionControllerComponent->GetComponentTransform().GetLocation() + targetMotionControllerComponent->GetForwardVector() * widgetInteractionComponent->InteractionDistance, screenLoc))
		{
			return false;
		}

		FVector worldLoc, worldDir;
		if (!DeprojectScreenPositionToWorld(screenLoc.X, screenLoc.Y, worldLoc, worldDir)) return false;
    	a_lineTraceStart = worldLoc;
    	a_lineTraceEnd = worldLoc + worldDir * 200000;
    	return true; 
	});

	Select(*this, delegate, m_DebugSelectionTrace, m_SelectionTraceRadius);
}

void AMSPPlayerController::OnInputVRGripLeft(const FInputActionValue &a_inputActionValue)
{
	OnInputVRGrip(a_inputActionValue, true);
}

void AMSPPlayerController::OnInputVRGripRight(const FInputActionValue &a_inputActionValue)
{
	OnInputVRGrip(a_inputActionValue, false);
}

void AMSPPlayerController::OnInputVRGrip(const FInputActionValue &a_inputActionValue, const bool a_isLeftMotionController)
{
	// do nothing, if already moving using the other motion controller
	if (m_moveState == (a_isLeftMotionController ? EVRMoveState::MovingWithRight : EVRMoveState::MovingWithLeft))
	{
		return;
	}
	AMSPPawn *pawn = GetPawn<AMSPPawn>();
	if (!pawn)
	{
		return;
	}
	if (const float val = a_inputActionValue.Get<float>(); val == 0.0f)
	{
		// movement released, back to idle move state
		m_moveState = EVRMoveState::Idle;
		return;
	}
	// get the motion controller, either left of right
	const UMotionControllerComponent *targetMotionControllerComponent(GetMotionControllerComponent(pawn, a_isLeftMotionController));
	if (!targetMotionControllerComponent)
	{
		return;
	}
	const FVector motionControllerForwardVector(targetMotionControllerComponent->GetForwardVector());
	// if there is a movement, add it, and set the correct move state
	if (motionControllerForwardVector != FVector::Zero())
	{
		pawn->AddMovementInput(motionControllerForwardVector);
		m_moveState = a_isLeftMotionController ? EVRMoveState::MovingWithLeft : EVRMoveState::MovingWithRight;
	}
}
