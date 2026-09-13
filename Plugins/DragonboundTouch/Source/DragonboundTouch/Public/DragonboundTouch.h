// DragonboundTouch — touch-first input plugin module.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DRAGONBOUNDTOUCH_API DECLARE_LOG_CATEGORY_EXTERN(LogDragonboundTouch, Log, All);

class FDragonboundTouchModule : public IModuleInterface
{
public:
	virtual void StartupModule() override {}
	virtual void ShutdownModule() override {}
};
