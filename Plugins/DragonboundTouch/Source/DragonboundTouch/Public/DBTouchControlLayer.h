// DragonboundTouch — touch control layer.
//
// Owned by ADBRiderPlayerController. Receives normalized input from the
// virtual controls widget and injects it into the shared Enhanced Input
// actions — the same actions the character binds. Gameplay code stays
// device-agnostic: whether a value arrived from a touchscreen, a gamepad,
// or a keyboard is invisible downstream (ADR-0003).

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DBTouchConfig.h"
#include "DBTouchControlLayer.generated.h"

class UEnhancedPlayerInput;

UCLASS(ClassGroup = (Input), Blueprintable, meta = (BlueprintSpawnableComponent))
class DRAGONBOUNDTOUCH_API UDBTouchControlLayer : public UActorComponent
{
	GENERATED_BODY()

public:
	UDBTouchControlLayer();

	virtual void BeginPlay() override;

	void SetTouchConfig(UDBTouchConfig* InConfig) { TouchConfig = InConfig; }

	UFUNCTION(BlueprintPure, Category = "Touch")
	UDBTouchConfig* GetTouchConfig() const { return TouchConfig; }

	/** Should the HUD create the virtual controls widget? (enabled + touch primary + local player) */
	UFUNCTION(BlueprintPure, Category = "Touch")
	bool ShouldCreateTouchControls() const;

	/** Touch is the primary input device (mobile platforms, or forced for testing). */
	UFUNCTION(BlueprintPure, Category = "Touch")
	bool IsTouchPrimary() const;

	// --- Virtual control callbacks (invoked by UDBTouchControlsWidget) ---

	/** Joystick vector in [-1,1], X right, Y forward. */
	void NotifyJoystickVector(const FVector2D& Vector);

	/** Look delta normalized against the reference height (sensitivity applied here). */
	void NotifyLookDelta(const FVector2D& NormalizedDelta);

	void NotifyButtonChanged(EDBTouchButton Button, bool bDown);

protected:
	void InjectAxis2D(UInputAction* Action, const FVector2D& Value);
	void InjectDigital(UInputAction* Action, bool bDown);
	UInputAction* GetActionForButton(EDBTouchButton Button) const;
	UEnhancedPlayerInput* GetEnhancedInput() const;

	/** Config asset (fallback: ADBRiderGameMode::DefaultTouchConfig, resolved by game code). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Touch")
	TObjectPtr<UDBTouchConfig> TouchConfig = nullptr;

	/** Last injected joystick vector — used to skip unchanged injections. */
	FVector2D LastInjectedJoystick = FVector2D::ZeroVector;
};
