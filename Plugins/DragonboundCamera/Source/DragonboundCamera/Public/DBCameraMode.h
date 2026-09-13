// DragonboundCamera — camera mode base class, perspective types, and POV math.
//
// Architecture (Docs/SYSTEMS.md §3, ADR-0002):
//   A UDBCameraDirectorComponent (on the PlayerController) owns one instance
//   of the active UDBCameraMode per perspective. Modes are pure config +
//   math: they compute a desired FDBPOV (location, rotation, FOV) each frame
//   from a FDBPOVContext. The director blends between modes (≤250 ms
//   crossfade through the view target's head position), applies collision
//   probes, and smooths the final POV. New contexts (Mounted, Flying — M6)
//   add new mode classes; nothing in the blend/director core changes.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DBCameraMode.generated.h"

/** Player-chosen perspective. The same enum is used for ground, mounted, and flight gameplay. */
UENUM(BlueprintType)
enum class EDBPerspective : uint8
{
	ThirdPerson,
	FirstPerson
};

/** Returns the opposite perspective (used by the toggle input). */
DRAGONBOUNDCAMERA_API EDBPerspective GetOppositePerspective(EDBPerspective Perspective);

/** A camera point of view: location, rotation, FOV. Engine-agnostic and blendable. */
USTRUCT(BlueprintType)
struct DRAGONBOUNDCAMERA_API FDBPOV
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "POV")
	FVector Location = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "POV")
	FRotator Rotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "POV")
	float FOV = 90.f;

	FDBPOV() {}
	FDBPOV(const FVector& InLocation, const FRotator& InRotation, float InFOV)
		: Location(InLocation), Rotation(InRotation), FOV(InFOV)
	{
	}

	/** Location lerp, quaternion slerp, FOV lerp. Alpha is clamped to [0,1]. */
	static FDBPOV Blend(const FDBPOV& A, const FDBPOV& B, float Alpha);
};

/** Immutable inputs handed to a camera mode when computing its desired POV. */
USTRUCT(BlueprintType)
struct DRAGONBOUNDCAMERA_API FDBPOVContext
{
	GENERATED_BODY()

	/** The actor being viewed (normally the possessed Rider). May be null on early frames. */
	const AActor* ViewTarget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FRotator ControlRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float DeltaTime = 0.f;

	FDBPOVContext() {}
	FDBPOVContext(const AActor* InViewTarget, const FRotator& InControlRotation, float InDeltaTime)
		: ViewTarget(InViewTarget), ControlRotation(InControlRotation), DeltaTime(InDeltaTime)
	{
	}
};

/**
 * Base camera mode. A mode is stateless configuration: it computes a desired
 * POV each frame. Combat/fly/dialogue variants derive from this class and
 * are registered per locomotion context on the camera director.
 */
UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew, CollapseCategories)
class DRAGONBOUNDCAMERA_API UDBCameraMode : public UObject
{
	GENERATED_BODY()

public:
	UDBCameraMode();

	/** Compute the desired POV for this frame. */
	virtual FDBPOV ComputePOV(const FDBPOVContext& Context) const;

	/** Pivot the camera orbits/probes from (actor location + PivotOffset by default). */
	virtual FVector GetPivotLocation(const FDBPOVContext& Context) const;

	/** Collision probe radius in world units; 0 disables probing for this mode. */
	virtual float GetProbeRadius() const { return 0.f; }

	/** True for first-person modes (director applies a near-wall micro-probe). */
	virtual bool IsFirstPersonMode() const { return false; }

	// --- Configuration (tuned per mode in Blueprint subclasses) ---

	/** Desired field of view. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	float FieldOfView = 90.f;

	/** Pitch clamp (degrees) for this mode. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	float PitchMin = -89.f;

	/** Pitch clamp (degrees) for this mode. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	float PitchMax = 89.f;

	/** Vertical offset from the view target's origin used for the pivot and the base POV. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	FVector PivotOffset = FVector(0.f, 0.f, 60.f);

	/** Steady-state smoothing speeds (per second). 0 = instant. Blend transitions ignore these. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Smoothing")
	float LocationInterpSpeed = 12.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Smoothing")
	float RotationInterpSpeed = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Smoothing")
	float FOVInterpSpeed = 8.f;
};

/**
 * Over-the-shoulder third-person mode: orbits a pivot above the character
 * at ArmLength with a small shoulder offset, probing against world geometry.
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew, CollapseCategories)
class DRAGONBOUNDCAMERA_API UDBCameraMode_ThirdPerson : public UDBCameraMode
{
	GENERATED_BODY()

public:
	UDBCameraMode_ThirdPerson();

	virtual FDBPOV ComputePOV(const FDBPOVContext& Context) const override;
	virtual float GetProbeRadius() const override { return ProbeRadius; }

	/** Distance behind the pivot. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	float ArmLength = 320.f;

	/** Lateral offset for the over-shoulder framing (positive = to the right). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	float ShoulderOffset = 30.f;

	/** Collision probe radius. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	float ProbeRadius = 12.f;
};

/**
 * First-person mode: places the camera at the view target's eye (skeletal
 * "head" socket when available) and applies a near-wall micro-probe.
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew, CollapseCategories)
class DRAGONBOUNDCAMERA_API UDBCameraMode_FirstPerson : public UDBCameraMode
{
	GENERATED_BODY()

public:
	UDBCameraMode_FirstPerson();

	virtual FDBPOV ComputePOV(const FDBPOVContext& Context) const override;
	virtual bool IsFirstPersonMode() const override { return true; }

	/** Socket used as the eye origin (UE mannequins use "head"). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	FName EyeSocketName = TEXT("head");

	/** Local-space offset applied to the eye socket location. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	FVector EyeOffset = FVector(0.f, 0.f, 10.f);

	/** If no eye socket exists, the eye sits this far below the capsule top. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	float EyeFallbackInset = 18.f;
};
