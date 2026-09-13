// DragonboundTouch — virtual touch controls widget implementation.

#include "DBTouchControlsWidget.h"
#include "DBTouchControlLayer.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "InputCoreTypes.h"
#include "Math/Box2D.h"

void UDBTouchControlsWidget::InitializeForLayer(const UDBTouchConfig* InConfig, UDBTouchControlLayer* InLayer)
{
	Config = InConfig;
	Layer = InLayer;
	// Layout is computed on the first tick once real geometry exists.
	LastViewportSize = FVector2D::ZeroVector;
}

UImage* UDBTouchControlsWidget::GetVisual(FName Name) const
{
	UWidget* Widget = GetWidgetFromName(Name);
	return Widget ? Cast<UImage>(Widget) : nullptr;
}

void UDBTouchControlsWidget::SetVisualRect(UImage* Image, const FVector2D& Position, const FVector2D& Size) const
{
	if (!Image)
	{
		return;
	}

	Image->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(Image->Slot))
	{
		Slot->SetPosition(Position);
		Slot->SetSize(Size);
	}
}

void UDBTouchControlsWidget::ApplyLayout(const FVector2D& ViewportSize)
{
	if (!Config)
	{
		return;
	}

	const float MinDim = FMath::Min(ViewportSize.X, ViewportSize.Y);
	SafeScale = MinDim / FMath::Max(Config->DpiReferenceHeight, 1.f);

	const float SafeLeft = Config->SafeAreaInsets.Left * SafeScale;
	const float SafeRight = Config->SafeAreaInsets.Right * SafeScale;
	const float SafeTop = Config->SafeAreaInsets.Top * SafeScale;
	const float SafeBottom = Config->SafeAreaInsets.Bottom * SafeScale;

	// --- Joystick (left zone) ---
	JoystickCenter = FVector2D(
		SafeLeft + Config->JoystickAnchorFraction.X * (ViewportSize.X - SafeLeft - SafeRight),
		ViewportSize.Y - SafeBottom - Config->JoystickAnchorFraction.Y * (ViewportSize.Y - SafeTop - SafeBottom));
	JoystickRadius = Config->JoystickRadiusFraction * MinDim;
	LookZoneStartX = SafeLeft + Config->JoystickZoneWidthFraction * (ViewportSize.X - SafeLeft - SafeRight);

	const float ThumbDiameter = Config->JoystickRadiusFraction * 0.36f * MinDim;
	SetVisualRect(GetVisual(TEXT("JoystickBase")),
		JoystickCenter - FVector2D(JoystickRadius, JoystickRadius),
		FVector2D(JoystickRadius * 2.f, JoystickRadius * 2.f));
	SetVisualRect(GetVisual(TEXT("JoystickThumb")),
		JoystickCenter - FVector2D(ThumbDiameter * 0.5f, ThumbDiameter * 0.5f),
		FVector2D(ThumbDiameter, ThumbDiameter));

	// --- Right-side look zone hint ---
	if (UImage* LookHint = GetVisual(TEXT("LookHint")))
	{
		LookHint->SetVisibility(ESlateVisibility::HitTestInvisible);
		LookHint->SetRenderOpacity(0.12f);
		if (UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(LookHint->Slot))
		{
			const FVector2D HintSize(MinDim * 0.3f, MinDim * 0.3f);
			Slot->SetPosition(FVector2D(
				ViewportSize.X - SafeRight - HintSize.X * 0.55f,
				SafeTop + (ViewportSize.Y - SafeTop - SafeBottom) * 0.4f));
			Slot->SetSize(HintSize);
		}
	}

	// --- Action buttons (2x2 cluster, bottom-right) ---
	const float ButtonSize = Config->ButtonSizeFraction * MinDim;
	const float Gap = Config->ButtonGapFraction * MinDim;
	const FVector2D ClusterSize(ButtonSize * 2.f + Gap, ButtonSize * 2.f + Gap);
	const FVector2D ClusterOrigin(
		ViewportSize.X - SafeRight - ClusterSize.X,
		ViewportSize.Y - SafeBottom - ClusterSize.Y);

	auto PlaceButton = [&](EDBTouchButton Button, int32 Col, int32 Row, FName VisualName)
	{
		const FVector2D Pos = ClusterOrigin + FVector2D(Col * (ButtonSize + Gap), Row * (ButtonSize + Gap));
		ButtonRects.Add(Button, FBox2D(Pos, Pos + FVector2D(ButtonSize, ButtonSize)));
		SetVisualRect(GetVisual(VisualName), Pos, FVector2D(ButtonSize, ButtonSize));
	};

	// Rows are bottom-up (row 0 = lowest, nearest the thumb).
	PlaceButton(EDBTouchButton::Jump, 1, 0, TEXT("ButtonJump"));
	PlaceButton(EDBTouchButton::Sprint, 0, 0, TEXT("ButtonSprint"));
	PlaceButton(EDBTouchButton::Interact, 1, 1, TEXT("ButtonInteract"));
	PlaceButton(EDBTouchButton::TogglePerspective, 0, 1, TEXT("ButtonCamera"));
}

bool UDBTouchControlsWidget::TryGetButtonAt(const FVector2D& LocalPos, EDBTouchButton& OutButton) const
{
	for (const auto& Pair : ButtonRects)
	{
		if (Pair.Value.IsInside(LocalPos))
		{
			OutButton = Pair.Key;
			return true;
		}
	}
	return false;
}

FVector2D UDBTouchControlsWidget::ComputeJoystickVector(const FVector2D& LocalPos) const
{
	const FVector2D Offset = LocalPos - JoystickCenter;
	const float Length = Offset.Size();
	const float Max = FMath::Max(JoystickRadius, 1.f);

	float Magnitude = FMath::Clamp(Length / Max, 0.f, 1.f);
	const float DeadZone = FMath::Clamp(Config ? Config->JoystickDeadZoneFraction : 0.08f, 0.f, 0.5f);

	if (Magnitude <= DeadZone || Length < KINDA_SMALL_NUMBER)
	{
		return FVector2D::ZeroVector;
	}

	Magnitude = (Magnitude - DeadZone) / (1.f - DeadZone);
	const FVector2D Direction = Offset / Length;
	// Screen Y grows downward; game input uses +Y = forward.
	return FVector2D(Direction.X * Magnitude, -Direction.Y * Magnitude);
}

void UDBTouchControlsWidget::RoutePointerDown(const FVector2D& LocalPos, int32 PointerId)
{
	if (!Layer)
	{
		return;
	}

	EDBTouchButton Button;
	if (TryGetButtonAt(LocalPos, Button))
	{
		if (!ButtonDown.FindRef(Button))
		{
			ButtonDown.Add(Button, true);
			ButtonPointers.Add(Button, PointerId);
			SetButtonPressedVisual(Button, true);
			Layer->NotifyButtonChanged(Button, true);
		}
		return;
	}

	if (!JoystickTouch.bActive && LocalPos.X <= LookZoneStartX)
	{
		JoystickTouch.bActive = true;
		JoystickTouch.PointerId = PointerId;
		JoystickTouch.Vector = FVector2D::ZeroVector;
		Layer->NotifyJoystickVector(FVector2D::ZeroVector);
		return;
	}

	if (!LookTouch.bActive && LocalPos.X > LookZoneStartX)
	{
		LookTouch.bActive = true;
		LookTouch.PointerId = PointerId;
		LookTouch.LastPosition = LocalPos;
	}
}

void UDBTouchControlsWidget::RoutePointerMove(const FVector2D& LocalPos, int32 PointerId)
{
	if (!Layer)
	{
		return;
	}

	if (JoystickTouch.bActive && JoystickTouch.PointerId == PointerId)
	{
		const FVector2D Vector = ComputeJoystickVector(LocalPos);
		if (!Vector.Equals(JoystickTouch.Vector))
		{
			JoystickTouch.Vector = Vector;
			UpdateJoystickThumbVisual(LocalPos);
			Layer->NotifyJoystickVector(Vector);
		}
	}
	else if (LookTouch.bActive && LookTouch.PointerId == PointerId)
	{
		const FVector2D Delta = LocalPos - LookTouch.LastPosition;
		LookTouch.LastPosition = LocalPos;

		if (!Delta.IsNearlyZero() && Config)
		{
			// Normalize against the reference height so sensitivity is
			// resolution-independent.
			const float Normalizer = Config->DpiReferenceHeight / FMath::Max(LastViewportSize.Y, 1.f);
			Layer->NotifyLookDelta(Delta * Normalizer);
		}
	}
}

void UDBTouchControlsWidget::RoutePointerUp(int32 PointerId)
{
	if (!Layer)
	{
		return;
	}

	if (JoystickTouch.bActive && JoystickTouch.PointerId == PointerId)
	{
		JoystickTouch.bActive = false;
		JoystickTouch.PointerId = INDEX_NONE;
		JoystickTouch.Vector = FVector2D::ZeroVector;
		UpdateJoystickThumbVisual(JoystickCenter);
		Layer->NotifyJoystickVector(FVector2D::ZeroVector);
		return;
	}

	if (LookTouch.bActive && LookTouch.PointerId == PointerId)
	{
		LookTouch.bActive = false;
		LookTouch.PointerId = INDEX_NONE;
		return;
	}

	for (const auto& Pair : ButtonPointers)
	{
		if (Pair.Value == PointerId)
		{
			const EDBTouchButton Button = Pair.Key;
			ButtonPointers.Remove(Button);
			ButtonDown.Remove(Button);
			SetButtonPressedVisual(Button, false);
			Layer->NotifyButtonChanged(Button, false);
			return;
		}
	}
}

void UDBTouchControlsWidget::UpdateJoystickThumbVisual(const FVector2D& LocalPos) const
{
	if (UImage* Thumb = GetVisual(TEXT("JoystickThumb")))
	{
		if (UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(Thumb->Slot))
		{
			const FVector2D Offset = LocalPos - JoystickCenter;
			const FVector2D Clamped = Offset.GetClampedToMaxSize(JoystickRadius);
			const FVector2D ThumbSize = Slot->GetSize();
			Slot->SetPosition(JoystickCenter + Clamped - ThumbSize * 0.5f);
		}
	}
}

void UDBTouchControlsWidget::SetButtonPressedVisual(EDBTouchButton Button, bool bPressed) const
{
	FName VisualName;
	switch (Button)
	{
	case EDBTouchButton::Jump:              VisualName = TEXT("ButtonJump"); break;
	case EDBTouchButton::Sprint:            VisualName = TEXT("ButtonSprint"); break;
	case EDBTouchButton::Interact:          VisualName = TEXT("ButtonInteract"); break;
	case EDBTouchButton::TogglePerspective: VisualName = TEXT("ButtonCamera"); break;
	default: return;
	}

	if (UImage* Visual = GetVisual(VisualName))
	{
		Visual->SetRenderOpacity(bPressed ? 0.55f : 1.f);
	}
}

void UDBTouchControlsWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const FVector2D ViewportSize = MyGeometry.GetLocalSize();
	if (!ViewportSize.Equals(LastViewportSize))
	{
		LastViewportSize = ViewportSize;
		ApplyLayout(ViewportSize);
	}
}

FReply UDBTouchControlsWidget::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InTouchEvent)
{
	RoutePointerDown(InGeometry.AbsoluteToLocal(InTouchEvent.GetScreenSpacePosition()), static_cast<int32>(InTouchEvent.GetPointerIndex()));
	return FReply::Handled();
}

FReply UDBTouchControlsWidget::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InTouchEvent)
{
	RoutePointerMove(InGeometry.AbsoluteToLocal(InTouchEvent.GetScreenSpacePosition()), static_cast<int32>(InTouchEvent.GetPointerIndex()));
	return FReply::Handled();
}

FReply UDBTouchControlsWidget::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InTouchEvent)
{
	RoutePointerUp(static_cast<int32>(InTouchEvent.GetPointerIndex()));
	return FReply::Handled();
}

FReply UDBTouchControlsWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		RoutePointerDown(InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()), 0);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply UDBTouchControlsWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		RoutePointerUp(0);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply UDBTouchControlsWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		RoutePointerMove(InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()), 0);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}
