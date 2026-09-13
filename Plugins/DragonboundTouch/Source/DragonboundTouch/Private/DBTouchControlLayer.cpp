// DragonboundTouch — touch control layer implementation.

#include "DBTouchControlLayer.h"
#include "CommonInputSubsystem.h"
#include "CommonInputBaseTypes.h"
#include "DragonboundTouch.h"
#include "EnhancedPlayerInput.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "HAL/IConsoleManager.h"

// Development/testing switch: force the touch layer on desktop (mouse simulates touch).
static TAutoConsoleVariable<bool> CVarDBTouchForceEnabled(
	TEXT("DB.TouchControls.Force"),
	false,
	TEXT("Force-enable Dragonbound touch controls (desktop testing)."),
	ECVF_Default);

UDBTouchControlLayer::UDBTouchControlLayer()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDBTouchControlLayer::BeginPlay()
{
	Super::BeginPlay();

	// Config resolution falls back to the game mode's DefaultTouchConfig
	// (set by ADBRiderPlayerController / ADBHUDBase, which live in the game
	// module — this plugin never depends on game code).
	if (!TouchConfig)
	{
		UE_LOG(LogDragonboundTouch, Verbose, TEXT("DBTouchControlLayer: no touch config — touch controls inactive."));
	}
}

bool UDBTouchControlLayer::IsTouchPrimary() const
{
	if (CVarDBTouchForceEnabled.GetValueOnGameThread())
	{
		return true;
	}

#if PLATFORM_ANDROID || PLATFORM_IOS
	return true;
#else
	const APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
	if (!PlayerController)
	{
		return false;
	}

	if (const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
	{
		if (const UCommonInputSubsystem* CommonInput = ULocalPlayer::GetSubsystem<UCommonInputSubsystem>(LocalPlayer))
		{
			return CommonInput->GetCurrentInputType() == ECommonInputType::Touch;
		}
	}

	return false;
#endif
}

bool UDBTouchControlLayer::ShouldCreateTouchControls() const
{
	if (!TouchConfig || !TouchConfig->bTouchEnabled || !TouchConfig->ControlsWidgetClass)
	{
		return false;
	}

	const APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
	return PlayerController && PlayerController->IsLocalController() && IsTouchPrimary();
}

UEnhancedPlayerInput* UDBTouchControlLayer::GetEnhancedInput() const
{
	const APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
	return PlayerController ? Cast<UEnhancedPlayerInput>(PlayerController->PlayerInput) : nullptr;
}

UInputAction* UDBTouchControlLayer::GetActionForButton(EDBTouchButton Button) const
{
	if (!TouchConfig)
	{
		return nullptr;
	}

	switch (Button)
	{
	case EDBTouchButton::Jump:               return TouchConfig->JumpAction;
	case EDBTouchButton::Sprint:             return TouchConfig->SprintAction;
	case EDBTouchButton::Interact:           return TouchConfig->InteractAction;
	case EDBTouchButton::TogglePerspective:  return TouchConfig->TogglePerspectiveAction;
	default:                                 return nullptr;
	}
}

void UDBTouchControlLayer::InjectAxis2D(UInputAction* Action, const FVector2D& Value)
{
	if (!Action)
	{
		return;
	}

	if (UEnhancedPlayerInput* EnhancedInput = GetEnhancedInput())
	{
		// Injected with empty modifier/trigger lists: touch-side scaling
		// (sensitivity, inversion, dead zone) is applied by this layer before
		// injection. Signature verified against UE 5.x Enhanced Input; the
		// whole injection surface lives in these two methods.
		const TArray<UInputModifier*> Modifiers;
		const TArray<UInputTrigger*> Triggers;
		EnhancedInput->InjectInputForAction(Action, FInputActionValue(Value), Modifiers, Triggers);
	}
}

void UDBTouchControlLayer::InjectDigital(UInputAction* Action, bool bDown)
{
	if (!Action)
	{
		return;
	}

	if (UEnhancedPlayerInput* EnhancedInput = GetEnhancedInput())
	{
		const TArray<UInputModifier*> Modifiers;
		const TArray<UInputTrigger*> Triggers;
		EnhancedInput->InjectInputForAction(Action, FInputActionValue(bDown), Modifiers, Triggers);
	}
}

void UDBTouchControlLayer::NotifyJoystickVector(const FVector2D& Vector)
{
	if (!TouchConfig || !TouchConfig->MoveAction)
	{
		return;
	}

	if (Vector.Equals(LastInjectedJoystick))
	{
		return;
	}

	LastInjectedJoystick = Vector;
	InjectAxis2D(TouchConfig->MoveAction, Vector);
}

void UDBTouchControlLayer::NotifyLookDelta(const FVector2D& NormalizedDelta)
{
	if (!TouchConfig || !TouchConfig->LookAction)
	{
		return;
	}

	FVector2D Delta = NormalizedDelta * TouchConfig->LookSensitivity;
	if (TouchConfig->bInvertLookY)
	{
		Delta.Y = -Delta.Y;
	}

	InjectAxis2D(TouchConfig->LookAction, Delta);
}

void UDBTouchControlLayer::NotifyButtonChanged(EDBTouchButton Button, bool bDown)
{
	InjectDigital(GetActionForButton(Button), bDown);
}
