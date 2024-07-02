#pragma once

#include "CoreMinimal.h"

#include "DataPoolTickActor.generated.h"

UCLASS()
class MSP_PROCEDURALGEN_API ADataPoolTickActor : public AActor
{
    GENERATED_BODY()

public:
    ADataPoolTickActor();

    virtual void Tick(float DeltaSeconds) override;

};