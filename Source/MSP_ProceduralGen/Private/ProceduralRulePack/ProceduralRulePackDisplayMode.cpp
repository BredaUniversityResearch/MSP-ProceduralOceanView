#include "ProceduralRulePackDisplayMode.h"

#include "ProceduralRulePackActor.h"

void UProceduralRulePackDisplayMode::EnableDisplayMode_Implementation(UDataPool* a_dataPool)
{
	check(!CurrentlyEnabled);
	CurrentlyEnabled = true;
	PendingEnableTimePoint = -1.0;
}

void UProceduralRulePackDisplayMode::DisableDisplayMode_Implementation(UDataPool* a_dataPool)
{
	check(CurrentlyEnabled || PendingEnableTimePoint > 0.0f);
	CurrentlyEnabled = false;
	PendingEnableTimePoint = -1.0;
}

void UProceduralRulePackDisplayMode::SetDisplayModePendingEnable(double a_enableTimePoint)
{
	PendingEnableTimePoint = a_enableTimePoint;
}

double UProceduralRulePackDisplayMode::GetDisplayModePendingEnableTime() const
{
	return PendingEnableTimePoint;
}

bool UProceduralRulePackDisplayMode::IsCurrentlyOrPendingEnabled() const
{
	return CurrentlyEnabled;
}

bool UProceduralRulePackDisplayMode::ShouldBeEnabledByDefault_Implementation() const
{
	return EnabledByDefault;
}

bool UProceduralRulePackDisplayMode::HasRequiredDataAvailable_Implementation(UDataPool* TargetDataPool) const
{
	return true;
}

FName UProceduralRulePackDisplayMode::GetDisplayModeName() const
{
	return GetFName();
}

FString UProceduralRulePackDisplayMode::GetHumanReadableName() const
{
	return HumanReadableName;
}

AProceduralRulePackActor* UProceduralRulePackDisplayMode::SpawnPRPActor(TSubclassOf<AProceduralRulePackActor> ActorToSpawn, FVector Positon)
{
	return  Cast<AProceduralRulePackActor>(GetWorld()->SpawnActor(ActorToSpawn.Get(), &Positon));
}
