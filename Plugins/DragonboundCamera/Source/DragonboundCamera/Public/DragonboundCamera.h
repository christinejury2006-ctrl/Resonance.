// DragonboundCamera — camera mode stack plugin module.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FDragonboundCameraModule : public IModuleInterface
{
public:
	virtual void StartupModule() override {}
	virtual void ShutdownModule() override {}
};
