// Dragonbound — base HUD implementation.

#include "UI/DBHUDBase.h"
#include "DBTouchControlLayer.h"
#include "DBTouchControlsWidget.h"
#include "Game/DBRiderGameMode.h"
#include "Player/DBRiderPlayerController.h"

void ADBHUDBase::BeginPlay()
{
	Super::BeginPlay();

	ADBRiderPlayerController* PlayerController = Cast<ADBRiderPlayerController>(GetOwningPlayerController());
	if (!PlayerController || !PlayerController->IsLocalPlayerController())
	{
		return;
	}

	UDBTouchControlLayer* Layer = PlayerController->GetTouchControlLayer();
	if (!Layer)
	{
		return;
	}

	// Fallback resolution (the controller also does this in its BeginPlay;
	// doing it here keeps the HUD self-sufficient regardless of begin order).
	if (!Layer->GetTouchConfig())
	{
		if (const ADBRiderGameMode* GameMode = GetWorld() ? Cast<ADBRiderGameMode>(GetWorld()->GetAuthGameMode()) : nullptr)
		{
			Layer->SetTouchConfig(GameMode->DefaultTouchConfig);
		}
	}

	if (!Layer->ShouldCreateTouchControls() || !Layer->GetTouchConfig() || !Layer->GetTouchConfig()->ControlsWidgetClass)
	{
		return;
	}

	TouchControlsWidget = CreateWidget<UDBTouchControlsWidget>(PlayerController, Layer->GetTouchConfig()->ControlsWidgetClass);
	if (TouchControlsWidget)
	{
		TouchControlsWidget->InitializeForLayer(Layer->GetTouchConfig(), Layer);
		TouchControlsWidget->AddToViewport(100);
	}
}
