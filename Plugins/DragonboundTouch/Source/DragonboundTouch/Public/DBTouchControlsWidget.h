// DragonboundTouch — virtual touch controls widget.
//
// Full-screen UMG widget that routes multi-touch into the touch control
// layer: left zone = movement joystick, right zone = drag-look, bottom-right
// = action buttons (jump/sprint/interact/camera). Layout is fractional and
// DPI-aware (slate units), so any aspect ratio (16:9 through 21:9) and any
// scale works; safe-area insets come from UDBTouchConfig.
//
// Visuals are optional child UImages looked up by name (see EDITOR_SETUP.md
// §9) — if they are missing, touch routing still works and nothing is drawn.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DBTouchConfig.h"
#include "DBTouchControlsWidget.generated.h"

class UImage;
class UDBTouchControlLayer;

UCLASS(Abstract, Blueprintable)
class DRAGONBOUNDTOUCH_API UDBTouchControlsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Bind config + layer; called by the HUD after widget creation. */
	void InitializeForLayer(const UDBTouchConfig* InConfig, UDBTouchControlLayer* InLayer);

protected:
	// --- UUserWidget ---

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InTouchEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InTouchEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InTouchEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	// --- Routing (touch and mouse share one path) ---

	void RoutePointerDown(const FVector2D& LocalPos, int32 PointerId);
	void RoutePointerMove(const FVector2D& LocalPos, int32 PointerId);
	void RoutePointerUp(int32 PointerId);

	// --- Layout ---

	void ApplyLayout(const FVector2D& ViewportSize);
	void SetVisualRect(UImage* Image, const FVector2D& Position, const FVector2D& Size) const;
	UImage* GetVisual(FName Name) const;
	bool TryGetButtonAt(const FVector2D& LocalPos, EDBTouchButton& OutButton) const;
	FVector2D ComputeJoystickVector(const FVector2D& LocalPos) const;

	// --- Visual feedback ---

	void UpdateJoystickThumbVisual(const FVector2D& LocalPos) const;
	void SetButtonPressedVisual(EDBTouchButton Button, bool bPressed) const;

	// --- State ---

	TObjectPtr<const UDBTouchConfig> Config;
	TObjectPtr<UDBTouchControlLayer> Layer;

	/** Cached layout (recomputed when the viewport size changes). */
	FVector2D LastViewportSize = FVector2D::ZeroVector;
	FVector2D JoystickCenter = FVector2D::ZeroVector;
	float JoystickRadius = 100.f;
	float LookZoneStartX = 0.f;
	float SafeScale = 1.f;

	TMap<EDBTouchButton, FBox2D> ButtonRects;
	TMap<EDBTouchButton, bool> ButtonDown;
	TMap<EDBTouchButton, int32> ButtonPointers;

	struct FJoystickTouch
	{
		bool bActive = false;
		int32 PointerId = INDEX_NONE;
		FVector2D Vector = FVector2D::ZeroVector;
	};
	FJoystickTouch JoystickTouch;

	struct FLookTouch
	{
		bool bActive = false;
		int32 PointerId = INDEX_NONE;
		FVector2D LastPosition = FVector2D::ZeroVector;
	};
	FLookTouch LookTouch;
};
