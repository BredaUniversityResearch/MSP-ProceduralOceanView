#pragma once

#include "JsonFileLoadHelper.h"
#include "Config/Config.h"

class UDataPool;

class FMSPConfigFileLoadHelper final : public JsonFileLoadHelper<UConfig>
{
protected:
	virtual void OnPostConvertJsonObjectToClass(TSharedPtr<FJsonObject> a_jsonObject, UConfig* a_result) override;
};