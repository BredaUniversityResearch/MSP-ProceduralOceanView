#include "MSPProcGenActorSelectable.h"

void AMSPProcGenActorSelectable::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// traverse the hierarchy of the actor searching for any primitive component and set it to block traces intended for MSPProcGenActorSelectable = ECC_GameTraceChannel3
	USceneComponent* rootComponent = GetRootComponent();
	TQueue<USceneComponent*> queue;
	queue.Enqueue(rootComponent);
	while (!queue.IsEmpty()) {
		// process next component
        USceneComponent* currentComponent;
        queue.Dequeue(currentComponent);
		if (UPrimitiveComponent* primitiveComponent = Cast<UPrimitiveComponent>(currentComponent)) {
			primitiveComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECollisionResponse::ECR_Block);
		}

		// add all child components of this component to the queue
        TArray<USceneComponent*> children;
        currentComponent->GetChildrenComponents(true, children);
        for (USceneComponent* child : children) {
            queue.Enqueue(child);
        }
	}
}

void AMSPProcGenActorSelectable::OnSelected_Implementation()
{
}

void AMSPProcGenActorSelectable::OnDoubleClickedEvent_Implementation()
{
}

void AMSPProcGenActorSelectable::OnDeselected_Implementation()
{
}
