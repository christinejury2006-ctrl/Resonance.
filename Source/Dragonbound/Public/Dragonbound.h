// Dragonbound — primary game module.
//
// M1 status: Rider framework (movement, camera modes, input) — see
// Docs/DEVELOPMENT.md and Docs/decisions/0002-camera-and-input.md.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DRAGONBOUND_API DECLARE_LOG_CATEGORY_EXTERN(LogDragonbound, Log, All);

class FDragonboundModule : public FDefaultGameModuleImpl
{
};
