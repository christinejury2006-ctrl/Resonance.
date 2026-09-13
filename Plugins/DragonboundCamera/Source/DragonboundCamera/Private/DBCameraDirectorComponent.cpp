// DragonboundCamera — camera director implementation.

#include "DBCameraDirectorComponent.h"
#include "Camera/CameraComponent.h"
#include "CollisionQueryParams.h"
#include "CollisionShape.h"
#include "DragonboundCameraLog.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#if ENABLE_DRAW_DEBUG
#include "DrawDebugHelpers.h"
#endif

UDBCameraDirectorComponent::UDBCameraDirectorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;

	ThirdPersonModeClass = UDBCameraMode_ThirdPerson::StaticClass();
	FirstPersonModeClass = UDBCameraMode_FirstPerson::StaticClass();
}

APlayerController* UDBCameraDirectorComponent::GetPlayerController() const
{
	return Cast<APlayerController>(GetOwner());
}

void UDBCameraDirectorComponent::GetPitchLimits(float& OutMinPitch, float& OutMaxPitch) const
{
	if (ActiveMode)
	{
		OutMinPitch = ActiveMode->PitchMin;
		OutMaxPitch = ActiveMode->PitchMax;
		return;
	}
	OutMinPitch = -89.f;
	OutMaxPitch = 89.f;
}

TSubclassOf<UDBCameraMode> UDBCameraDirectorComponent::ResolveModeClass(EDBPerspective InPerspective) const
{
	if (const FDBPerspectiveModePair* Pair = ContextModes.Find(LocomotionContext))
	{
		const TSubclassOf<UDBCameraMode> ContextClass =
			InPerspective == EDBPerspective::ThirdPerson ? Pair->ThirdPersonMode : Pair->FirstPersonMode;
		if (ContextClass)
		{
			return ContextClass;
		}
	}

	return InPerspective == EDBPerspective::ThirdPerson ? ThirdPersonModeClass : FirstPersonModeClass;
}

UDBCameraMode* UDBCameraDirectorComponent::GetOrCreateMode(TSubclassOf<UDBCameraMode> ModeClass)
{
	if (!ModeClass)
	{
		return nullptr;
	}

	for (TObjectPtr<UDBCameraMode>& Existing : InstancedModes)
	{
		if (Existing && Existing->GetClass() == ModeClass)
		{
			return Existing;
		}
	}

	UDBCameraMode* NewMode = NewObject<UDBCameraMode>(this, ModeClass);
	InstancedModes.Add(NewMode);
	return NewMode;
}

void UDBCameraDirectorComponent::BeginBlend(EDBPerspective TargetPerspective, TSubclassOf<UDBCameraMode> TargetClass)
{
	if (!TargetClass)
	{
		TargetClass = ResolveModeClass(TargetPerspective);
	}

	UDBCameraMode* TargetMode = GetOrCreateMode(TargetClass);
	if (!TargetMode)
	{
		UE_LOG(LogDragonboundCamera, Warning, TEXT("CameraDirector: no mode resolved for perspective %d"), static_cast<int32>(TargetPerspective));
		return;
	}

	BlendStartPOV = CurrentPOV;
	BlendTargetMode = TargetMode;
	bBlending = true;
	BlendAlpha = 0.f;

	if (Perspective != TargetPerspective)
	{
		Perspective = TargetPerspective;
		OnPerspectiveChanged.Broadcast(Perspective);
	}
}

void UDBCameraDirectorComponent::TogglePerspective()
{
	SetPerspective(GetOppositePerspective(Perspective));
}

void UDBCameraDirectorComponent::SetPerspective(EDBPerspective NewPerspective)
{
	if (!bBlending && NewPerspective == Perspective)
	{
		return;
	}
	BeginBlend(NewPerspective);
}

void UDBCameraDirectorComponent::SetLocomotionContext(FGameplayTag ContextTag)
{
	if (LocomotionContext == ContextTag)
	{
		return;
	}

	LocomotionContext = ContextTag;

	// If the new context resolves to a different mode for the current
	// perspective, blend into it (same perspective, different framing).
	const TSubclassOf<UDBCameraMode> Resolved = ResolveModeClass(Perspective);
	if (Resolved && (!ActiveMode || ActiveMode->GetClass() != Resolved))
	{
		BeginBlend(Perspective, Resolved);
	}
}

void UDBCameraDirectorComponent::ApplyCollision(FDBPOV& POV, const UDBCameraMode* Mode, const FDBPOVContext& Context) const
{
	if (!Mode)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	FCollisionQueryParams Params(FName(TEXT("DBCameraProbe")), false, Context.ViewTarget);
	FHitResult Hit;

	if (Mode->IsFirstPersonMode())
	{
		// Micro-probe: keep the eye from clipping into nearby walls.
		const float ProbeRadius = 6.f;
		const float ProbeDistance = 12.f;
		const FVector End = POV.Location + POV.Rotation.Vector() * ProbeDistance;

		if (World->SweepSingleByChannel(Hit, POV.Location, End, FQuat::Identity, ECC_Camera,
			FCollisionShape::MakeSphere(ProbeRadius), Params))
		{
			POV.Location = Hit.Location + Hit.Normal * ProbeRadius;
		}
	}
	else if (Mode->GetProbeRadius() > 0.f)
	{
		// Orbit probe: sweep from the pivot toward the desired camera location.
		const float ProbeRadius = Mode->GetProbeRadius();
		const FVector Pivot = Mode->GetPivotLocation(Context);

		if (World->SweepSingleByChannel(Hit, Pivot, POV.Location, FQuat::Identity, ECC_Camera,
			FCollisionShape::MakeSphere(ProbeRadius), Params))
		{
			POV.Location = Hit.Location + Hit.Normal * ProbeRadius;

#if ENABLE_DRAW_DEBUG
			if (bDrawDebug)
			{
				DrawDebugSphere(World, Hit.Location, 4.f, 8, FColor::Red, false, -1.f, 0, 0.5f);
			}
#endif
		}

#if ENABLE_DRAW_DEBUG
		if (bDrawDebug)
		{
			DrawDebugLine(World, Pivot, POV.Location, FColor::Green, false, -1.f, 0, 0.5f);
		}
#endif
	}
}

void UDBCameraDirectorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APlayerController* PC = GetPlayerController();
	AActor* ViewTarget = PC ? PC->GetViewTarget() : nullptr;
	if (!ViewTarget)
	{
		return;
	}

	if (!ActiveMode)
	{
		ActiveMode = GetOrCreateMode(ResolveModeClass(Perspective));
		if (!ActiveMode)
		{
			return;
		}
	}

	if (!bHasInitializedPOV)
	{
		// First camera frame: there is no history to blend from, so jump
		// straight to the target mode instead of blending from the origin.
		if (bBlending && BlendTargetMode)
		{
			ActiveMode = BlendTargetMode;
			BlendTargetMode = nullptr;
		}
		bBlending = false;
	}

	const FDBPOVContext Context(ViewTarget, PC->GetControlRotation(), DeltaTime);

	FDBPOV DesiredPOV;
	UDBCameraMode* CollisionMode = ActiveMode;

	if (bBlending && BlendTargetMode)
	{
		const FDBPOV TargetPOV = BlendTargetMode->ComputePOV(Context);
		DesiredPOV = FDBPOV::Blend(BlendStartPOV, TargetPOV, BlendAlpha);

		BlendAlpha += (PerspectiveBlendTime > KINDA_SMALL_NUMBER) ? (DeltaTime / PerspectiveBlendTime) : 1.f;
		if (BlendAlpha >= 1.f)
		{
			bBlending = false;
			ActiveMode = BlendTargetMode;
			BlendTargetMode = nullptr;
		}

		CollisionMode = bBlending ? BlendTargetMode : ActiveMode;
	}
	else
	{
		DesiredPOV = ActiveMode->ComputePOV(Context);
	}

	// Clamp pitch to the active mode's limits.
	DesiredPOV.Rotation.Pitch = FMath::ClampAngle(DesiredPOV.Rotation.Pitch, ActiveMode->PitchMin, ActiveMode->PitchMax);

	// Collision is applied to the desired POV before smoothing so walls
	// hard-stop the camera; smoothing only eases the recovery.
	ApplyCollision(DesiredPOV, CollisionMode, Context);

	if (!bHasInitializedPOV)
	{
		CurrentPOV = DesiredPOV;
		bHasInitializedPOV = true;
	}
	else if (!bBlending && ActiveMode)
	{
		const float LocSpeed = ActiveMode->LocationInterpSpeed;
		const float RotSpeed = ActiveMode->RotationInterpSpeed;
		const float FOVSpeed = ActiveMode->FOVInterpSpeed;

		CurrentPOV.Location = LocSpeed > 0.f ? FMath::VInterpTo(CurrentPOV.Location, DesiredPOV.Location, DeltaTime, LocSpeed) : DesiredPOV.Location;
		CurrentPOV.Rotation = RotSpeed > 0.f ? FMath::RInterpTo(CurrentPOV.Rotation, DesiredPOV.Rotation, DeltaTime, RotSpeed) : DesiredPOV.Rotation;
		CurrentPOV.FOV = FOVSpeed > 0.f ? FMath::FInterpTo(CurrentPOV.FOV, DesiredPOV.FOV, DeltaTime, FOVSpeed) : DesiredPOV.FOV;
	}
	else
	{
		// During a blend, the blend itself is the transition; no extra smoothing.
		CurrentPOV = DesiredPOV;
	}

	// Keep the controller's camera component in sync (used for post-process,
	// effects, and interaction traces).
	if (UCameraComponent* CameraComponent = PC->FindComponentByClass<UCameraComponent>())
	{
		CameraComponent->SetWorldLocationAndRotation(CurrentPOV.Location, CurrentPOV.Rotation);
		CameraComponent->SetFieldOfView(CurrentPOV.FOV);
	}
}
