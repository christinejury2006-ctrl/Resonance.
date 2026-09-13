// Dragonbound — Rider player controller implementation.

#include "Player/DBRiderPlayerController.h"
#include "Camera/CameraComponent.h"
#include "DBCameraDirectorComponent.h"
#include "DBTouchControlLayer.h"
#include "Game/DBRiderGameMode.h"
#include "Interaction/DBInteractionComponent.h"
#include "Player/DBPlayerCameraManager.h"

ADBRiderPlayerController::ADBRiderPlayerController()
{
	CameraDirector = CreateDefaultSubobject<UDBCameraDirectorComponent>(TEXT("CameraDirector"));
	InteractionComponent = CreateDefaultSubobject<UDBInteractionComponent>(TEXT("InteractionComponent"));
	TouchControlLayer = CreateDefaultSubobject<UDBTouchControlLayer>(TEXT("TouchControlLayer"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	PlayerCameraManagerClass = ADBPlayerCameraManager::StaticClass();
}

void ADBRiderPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Touch config falls back to the game mode's DefaultTouchConfig so the
	// asset is authored once (EDITOR_SETUP.md §9).
	if (TouchControlLayer && !TouchControlLayer->GetTouchConfig())
	{
		if (const ADBRiderGameMode* GameMode = GetWorld() ? Cast<ADBRiderGameMode>(GetWorld()->GetAuthGameMode()) : nullptr)
		{
			TouchControlLayer->SetTouchConfig(GameMode->DefaultTouchConfig);
		}
	}
}
