// DragonboundTouch — touch controls configuration data asset.
//
// All touch behavior is data (ADR-0003): layout, sizing, safe-area margins,
// sensitivity, and the input actions the virtual controls feed. The actions
// are the SAME assets used by the keyboard/mouse and gamepad mapping
// contexts (authored once per EDITOR_SETUP.md) — touch is one more source
// for the identical Enhanced Input pipeline.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Layout/Margin.h"
#include "DBTouchConfig.generated.h"

class UInputAction;
class UDBTouchControlsWidget;

/** The touch action buttons (arranged bottom-right, 2x2, in this order). */
UENUM(BlueprintType)
enum class EDBTouchButton : uint8
{
	Jump,
	Sprint,
	Interact,
	TogglePerspective,
	Count
};

UCLASS(BlueprintType, Blueprintable)
class DRAGONBOUNDTOUCH_API UDBTouchConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	// --- Input actions (same assets as DB_InputConfig — shared pipeline) ---

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Touch|Actions")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Touch|Actions")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Touch|Actions")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Touch|Actions")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Touch|Actions")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Touch|Actions")
	TObjectPtr<UInputAction> TogglePerspectiveAction;

	// --- Left virtual joystick ---

	/** Fraction of the viewport width reserved for the movement joystick zone. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Touch|Joystick", meta = (ClampMin = "0.1", ClampMax = "0.9"))
	float JoystickZoneWidthFraction = 0.5f;

	/** Joystick travel radius as a fraction of the smaller viewport dimension. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Touch|Joystick", meta = (ClampMin = "0.02", ClampMax = "0.5"))
	float JoystickRadiusFraction = 0.13f;

	/** Dead zone as a fraction of the joystick radius (below this, input is zero). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Touch|Joystick", meta = (ClampMin = "0.0", ClampMax = "0.5"))
	float JoystickDeadZoneFraction = 0.08f;

	/** Joystick base anchor: fraction across the viewport, and up from the bottom. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Touch|Joystick", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	FVector2D JoystickAnchorFraction = FVector2D(0.24f, 0.78f);

	// --- Right-side drag look ---

	/** Look sensitivity: degrees per normalized screen unit (1.0 = reference height of pixels dragged). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Touch|Look", meta = (ClampMin = "0.01"))
	float LookSensitivity = 0.35f;

	/** Drag up = look up. Independent of the KBM/gamepad modifier chain. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Touch|Look")
	bool bInvertLookY = true;

	/** Reference viewport height used to normalize drag deltas (sensitivity stays consistent across resolutions). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Touch|Look", meta = (ClampMin = "1.0"))
	float DpiReferenceHeight = 1080.f;

	// --- Layout, safe areas, buttons ---

	/** Safe-area margins (notches, rounded corners, home indicators) in reference-1080 units. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Touch|Layout")
	FMargin SafeAreaInsets = FMargin(24.f, 32.f, 24.f, 16.f);

	/** Button size as a fraction of the smaller viewport dimension. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Touch|Layout", meta = (ClampMin = "0.03", ClampMax = "0.3"))
	float ButtonSizeFraction = 0.10f;

	/** Gap between buttons as a fraction of the smaller viewport dimension. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Touch|Layout", meta = (ClampMin = "0.0", ClampMax = "0.2"))
	float ButtonGapFraction = 0.02f;

	// --- Activation ---

	/** Master switch for the touch layer. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Touch")
	bool bTouchEnabled = true;

	/** Widget Blueprint presenting the virtual controls (parent: UDBTouchControlsWidget). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Touch")
	TSubclassOf<UDBTouchControlsWidget> ControlsWidgetClass;
};
