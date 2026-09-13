// Dragonbound — Rider game mode implementation.

#include "Game/DBRiderGameMode.h"
#include "Characters/DBRiderCharacter.h"
#include "Game/DBPlayerStateBase.h"
#include "Player/DBRiderPlayerController.h"
#include "UI/DBHUDBase.h"

ADBRiderGameMode::ADBRiderGameMode()
{
	DefaultPawnClass = ADBRiderCharacter::StaticClass();
	PlayerControllerClass = ADBRiderPlayerController::StaticClass();
	PlayerStateClass = UDBPlayerStateBase::StaticClass();
	HUDClass = ADBHUDBase::StaticClass();
}
