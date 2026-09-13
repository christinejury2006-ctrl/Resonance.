// Dragonbound — player camera manager.
//
// The camera manager's only job is to publish the camera director's blended
// POV as the rendered view target. The director owns all camera logic.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "DBPlayerCameraManager.generated.h"

UCLASS()
class DRAGONBOUND_API ADBPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
};
