// Dragonbound — Rider player controller implementation.

#include "Player/DBRiderPlayerController.h"
#include "Camera/CameraComponent.h"
#include "DBCameraDirectorComponent.h"
#include "Interaction/DBInteractionComponent.h"
#include "Player/DBPlayerCameraManager.h"

ADBRiderPlayerController::ADBRiderPlayerController()
{
	CameraDirector = CreateDefaultSubobject<UDBCameraDirectorComponent>(TEXT("CameraDirector"));
	InteractionComponent = CreateDefaultSubobject<UDBInteractionComponent>(TEXT("InteractionComponent"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	PlayerCameraManagerClass = ADBPlayerCameraManager::StaticClass();
}
