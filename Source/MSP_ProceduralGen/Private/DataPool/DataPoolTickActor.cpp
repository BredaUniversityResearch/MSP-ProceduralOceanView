#include "DataPoolTickActor.h"

#include "MSPProcGenGameInstance.h"
#include "DataPool/DataPool.h"

ADataPoolTickActor::ADataPoolTickActor() : 
    Super()
{
    
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickGroup = TG_PostUpdateWork;
}

void ADataPoolTickActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    auto& dataPool = UMSPProcGenGameInstance::GetInstance()->GetDataPool();

    dataPool.ProcessChangedEvents();

}



