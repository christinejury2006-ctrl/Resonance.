// DragonboundCamera — camera director.
//
// Owned by the PlayerController. Owns the active camera mode instances,
// computes the blended/collision-safe POV every tick, and routes locomotion
// context changes (OnFoot / Mounted / Flying) to mode selection. This is the
// single source of truth for the player camera — see ADR-0002.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "DBCameraMode.h"
#include "DBCameraDirectorComponent.generated.h"

/** Broadcast when the active perspective changes (after a toggle). */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDBPerspectiveChangedSignature, EDBPerspective, Perspective);

/** The two modes that serve one locomotion context (e.g. OnFoot, Mounted, Flying). */
USTRUCT(BlueprintType)
struct FDBPerspectiveModePair
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<UDBCameraMode> ThirdPersonMode = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TSubclassOf<UDBCameraMode> FirstPersonMode = nullptr;
};

/**
 * Camera director: perspective state, mode selection per locomotion context,
 * POV blending, collision probing, and steady-state smoothing.
 */
UCLASS(ClassGroup = (Camera), Blueprintable, meta = (BlueprintSpawnableComponent))
class DRAGONBOUNDCAMERA_API UDBCameraDirectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDBCameraDirectorComponent();

	// --- Query API ---

	UFUNCTION(BlueprintPure, Category = "Camera")
	EDBPerspective GetPerspective() const { return Perspective; }

	/** The POV that the camera manager should render (updated every tick). */
	const FDBPOV& GetLatestPOV() const { return CurrentPOV; }

	/** Pitch limits of the active mode, for look-input clamping. */
	UFUNCTION(BlueprintPure, Category = "Camera")
	void GetPitchLimits(float& OutMinPitch, float& OutMaxPitch) const;

	UFUNCTION(BlueprintPure, Category = "Camera")
	bool ShouldHideRiderBody() const { return bHideRiderBodyInFirstPerson; }

	// --- Command API ---

	/** Swap between first and third person (the single toggle input). */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void TogglePerspective();

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void SetPerspective(EDBPerspective NewPerspective);

	/**
	 * Route a locomotion context change (DB.Locomotion.OnFoot/Mounted/Flying).
	 * Resolves the mode classes for the context (ContextModes map, falling
	 * back to the default classes) and blends if the active mode changes.
	 */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void SetLocomotionContext(FGameplayTag ContextTag);

	UFUNCTION(BlueprintPure, Category = "Camera")
	FGameplayTag GetLocomotionContext() const { return LocomotionContext; }

	// --- Events ---

	UPROPERTY(BlueprintAssignable, Category = "Camera")
	FDBPerspectiveChangedSignature OnPerspectiveChanged;

	// --- Config ---

	/** Default third-person mode (used when a context has no explicit pair). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	TSubclassOf<UDBCameraMode> ThirdPersonModeClass;

	/** Default first-person mode. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	TSubclassOf<UDBCameraMode> FirstPersonModeClass;

	/** Per-locomotion-context mode pairs. M1 uses the defaults (OnFoot); Mounted/Flying add entries later. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	TMap<FGameplayTag, FDBPerspectiveModePair> ContextModes;

	/** Crossfade duration for perspective/context switches (target ≤250 ms). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	float PerspectiveBlendTime = 0.25f;

	/** Whether the Rider's full-body mesh is hidden in first person (via IDBPerspectiveAware). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	bool bHideRiderBodyInFirstPerson = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera|Debug")
	bool bDrawDebug = false;

	// --- UActorComponent ---

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	/** Resolve the mode class for a perspective under the current locomotion context. */
	TSubclassOf<UDBCameraMode> ResolveModeClass(EDBPerspective InPerspective) const;

	/** Get (or create) the cached mode instance for a class. */
	UDBCameraMode* GetOrCreateMode(TSubclassOf<UDBCameraMode> ModeClass);

	/** Begin a POV blend toward a perspective (optionally with an explicit mode class). */
	void BeginBlend(EDBPerspective TargetPerspective, TSubclassOf<UDBCameraMode> TargetClass = nullptr);

	/** Apply the active mode's collision probe to a computed POV. */
	void ApplyCollision(FDBPOV& POV, const UDBCameraMode* Mode, const FDBPOVContext& Context) const;

	APlayerController* GetPlayerController() const;

protected:
	UPROPERTY(Transient)
	TObjectPtr<UDBCameraMode> ActiveMode = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UDBCameraMode> BlendTargetMode = nullptr;

	/** Instances cached by class; modes are stateless config so they are reused. */
	UPROPERTY(Transient)
	TArray<TObjectPtr<UDBCameraMode>> InstancedModes;

	EDBPerspective Perspective = EDBPerspective::ThirdPerson;

	FGameplayTag LocomotionContext;

	FDBPOV CurrentPOV;
	FDBPOV BlendStartPOV;
	float BlendAlpha = 1.f;
	bool bBlending = false;
	bool bHasInitializedPOV = false;
};
