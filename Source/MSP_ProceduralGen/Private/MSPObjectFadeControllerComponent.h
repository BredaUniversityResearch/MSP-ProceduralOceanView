#pragma once

#include "MSPObjectFadeControllerComponent.generated.h"

UENUM(BlueprintType)
enum class EObjectFadeState : uint8
{
    FadingIn,
    FadingOut,
    Idle
};

UCLASS(BlueprintType)
class UMSPObjectFadeControllerComponent : public UActorComponent
{
    GENERATED_BODY()

public:

    UMSPObjectFadeControllerComponent();


    virtual void BeginPlay() override;
    virtual void TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;



    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void StartFadeIn( const TArray<UMaterialInstanceDynamic*>& Materials );

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void StartFadeOut( const TArray<UMaterialInstanceDynamic*>& Materials );

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FadeInTime = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FadeOutTime = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FadeInDelay = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FadeOutDelay = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool EnableFadeController = false;

private:
    
    UPROPERTY()
    TArray<UMaterialInstanceDynamic*> MaterialsToFade = {};

    UPROPERTY()
    EObjectFadeState CurrentFadeState = EObjectFadeState::Idle;

    UPROPERTY()
    float CurrentFadeTime = 0.0f;
};
