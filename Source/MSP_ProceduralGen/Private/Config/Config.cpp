#include "Config/Config.h"

#include "MSPProcGenGameMode.h"

UConfig* UConfig::GetInstance()
{
	AMSPProcGenGameMode* gameMode = Cast<AMSPProcGenGameMode>(GWorld->GetAuthGameMode());
	return gameMode->GetLoadedConfig();
}
