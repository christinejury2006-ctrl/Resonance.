// Dragonbound — player camera manager implementation.

#include "Player/DBPlayerCameraManager.h"
#include "DBCameraDirectorComponent.h"
#include "GameFramework/PlayerController.h"

void ADBPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && OutVT.Target && OutVT.Target == PlayerController->GetPawn())
	{
		if (const UDBCameraDirectorComponent* Director = PlayerController->FindComponentByClass<UDBCameraDirectorComponent>())
		{
			const FDBPOV& POV = Director->GetLatestPOV();

			OutVT.POV.Location = POV.Location;
			OutVT.POV.Rotation = POV.Rotation;
			OutVT.POV.FOV = POV.FOV;
			OutVT.bOverrideFOV = true;
			return;
		}
	}

	Super::UpdateViewTarget(OutVT, DeltaTime);
}
