#pragma once

#include "InputAction.h"
#include "MSPPlayerController.generated.h"

class AMSPProcGenActorSelectable;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerLookHoldEvent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerSelectEvent, AMSPProcGenActorSelectable*, selectedActor);

DECLARE_DELEGATE_RetVal_TwoParams(bool, FGetLineTranceStartAndEnd, FVector &, FVector &);

UCLASS()
class AMSPPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable)
	FPlayerLookHoldEvent PlayerLookHoldStarted;
	UPROPERTY(BlueprintAssignable)
	FPlayerLookHoldEvent PlayerLookHoldFinished;

	UPROPERTY(BlueprintAssignable)
	FPlayerSelectEvent OnPlayerSelectObject;

	UFUNCTION(BlueprintCallable)
	void SetSelectedActor(AMSPProcGenActorSelectable* a_selectedActorReference);

protected:
	virtual void SetupInputComponent() override;

private:
	void OnPlayerMove(const FInputActionValue& a_axisValue);
	void OnPlayerMoveUpDown(const FInputActionValue& a_inputActionValue);
	void OnInputMouseLook(const FInputActionValue& a_inputActionValue);
	void OnInputLookHold(const FInputActionValue& a_inputActionValue);
	void OnInputSelect(const FInputActionValue& a_inputActionValue);
	void OnInputAdjustMoveSpeed(const FInputActionValue& a_inputActionValue);
	void OnInputNextCameraPreset(const FInputActionValue& a_inputActionValue);
	void OnInputDoubleClick(const FInputActionValue& a_inputActionValue);
	void OnInputCamera1(const FInputActionValue& a_inputActionValue);
	void OnInputCamera2(const FInputActionValue& a_inputActionValue);
	void OnInputCamera3(const FInputActionValue& a_inputActionValue);
	void OnInputCamera4(const FInputActionValue& a_inputActionValue);
	void OnInputFocusObject(const FInputActionValue& a_inputActionValue);

	// VR specific
	void OnInputVRSelectLeft(const FInputActionValue& a_inputActionValue);
	void OnInputVRSelectRight(const FInputActionValue& a_inputActionValue);
	void OnInputVRSelect(const FInputActionValue& a_inputActionValue, const bool a_isLeftMotionController);
	void OnInputVRAdjustMoveSpeed(const FInputActionValue& a_axisValue);
	void OnInputVRGripLeft(const FInputActionValue& a_inputActionValue);
	void OnInputVRGripRight(const FInputActionValue& a_inputActionValue);
	void OnInputVRGrip(const FInputActionValue& a_inputActionValue, const bool a_isLeftMotionController);

	void Select(const AMSPPlayerController &a_controller, FGetLineTranceStartAndEnd a_getLineTraceStartAndEndDelegate, const bool a_debugTrace, const float a_traceRadius );

	UFUNCTION()
	void OnDisplayModeToggled(FName a_displayModeName, bool a_currentlyEnabled);

	UPROPERTY(EditDefaultsOnly)
	UInputAction* InputActionMove;
	UPROPERTY(EditDefaultsOnly);
	UInputAction* InputActionMoveUpDown;
	UPROPERTY(EditDefaultsOnly);
	UInputAction* InputActionMouseLook;
	UPROPERTY(EditDefaultsOnly);
	UInputAction* InputActionLookHold;
	UPROPERTY(EditDefaultsOnly);
	UInputAction* InputActionSelect;
	UPROPERTY(EditDefaultsOnly);
	UInputAction* InputActionAdjustMoveSpeed;
	UPROPERTY(EditDefaultsOnly);
	UInputAction* InputActionNextCameraPreset;
	UPROPERTY(EditDefaultsOnly);
	UInputAction* InputActionDoubleClick;
	UPROPERTY(EditDefaultsOnly);
	UInputAction* InputActionCamera1;
	UPROPERTY(EditDefaultsOnly);
	UInputAction* InputActionCamera2;
	UPROPERTY(EditDefaultsOnly);
	UInputAction* InputActionCamera3;
	UPROPERTY(EditDefaultsOnly);
	UInputAction* InputActionCamera4;
	UPROPERTY(EditDefaultsOnly);
	UInputAction* InputActionFocusObject;


	// VR specific
	UPROPERTY(EditDefaultsOnly);
	UInputAction* InputActionVRSelectLeft;
	UPROPERTY(EditDefaultsOnly);
	UInputAction* InputActionVRSelectRight;
	UPROPERTY(EditDefaultsOnly);
	UInputAction* InputActionVRAdjustMoveSpeed;
	UPROPERTY(EditDefaultsOnly);
	UInputAction* InputActionVRGripLeft;
	UPROPERTY(EditDefaultsOnly);
	UInputAction* InputActionVRGripRight;

	UPROPERTY(EditDefaultsOnly);
	bool m_DebugSelectionTrace = false;
	UPROPERTY(EditDefaultsOnly);
	float m_SelectionTraceRadius = { 500.0f };

	bool m_shouldProcessMouseMove{ false };
	FVector m_mouseDownPosition;
	int m_currentCameraPreset{ 0 };

	uint32 m_clickCount = { 0 };
	UPROPERTY(EditDefaultsOnly);
	float m_resetClickCountDelaySec = { 0.2f };

	UPROPERTY()
	TWeakObjectPtr<AMSPProcGenActorSelectable> m_selectedActorReference;
	FTimerHandle m_resetClickCountTimerHandle;

	// VR specific
	enum class EVRMoveState
	{
		Idle,
		MovingWithLeft,
		MovingWithRight
	};
	EVRMoveState m_moveState = EVRMoveState::Idle;
};