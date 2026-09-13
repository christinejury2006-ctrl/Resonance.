// Dragonbound — the playable Rider.
//
// The player's character: Enhanced Input handling, camera-relative
// locomotion, perspective-aware body hiding, appearance resolution
// (male/female data assets), and the locomotion-context hook that
// Mounted/Flying states will drive in later milestones.
//
// Camera work lives on the controller's UDBCameraDirectorComponent
// (ADR-0002) — this class never manipulates the camera directly.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "GameplayTagContainer.h"
#include "DBCameraMode.h"
#include "DBPerspectiveAware.h"
#include "DBRiderCharacter.generated.h"

class ADBRiderCharacter;
class USkeletalMeshComponent;
class UDBInputConfig;
class UDBRiderAppearanceDefinition;
class UDBRiderMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDBLocomotionContextChangedSignature, ADBRiderCharacter*, Rider, FGameplayTag, PreviousContext, FGameplayTag, NewContext);

UCLASS()
class DRAGONBOUND_API ADBRiderCharacter : public ACharacter, public IDBPerspectiveAware
{
	GENERATED_BODY()

public:
	ADBRiderCharacter(const FObjectInitializer& ObjectInitializer);

	// --- Accessors ---

	UFUNCTION(BlueprintPure, Category = "Rider")
	UDBRiderMovementComponent* GetRiderMovementComponent() const;

	UFUNCTION(BlueprintPure, Category = "Rider")
	ERiderSex GetRiderSex() const { return RiderSex; }

	/** Current locomotion context tag (DB.Locomotion.*). OnFoot in M1. */
	UFUNCTION(BlueprintPure, Category = "Rider")
	FGameplayTag GetLocomotionContext() const { return LocomotionContext; }

	/**
	 * Set the locomotion context (OnFoot/Mounted/Flying). Routes the camera
	 * director and broadcasts for input/AI/UI listeners. Mounted and Flying
	 * are driven by the dragon systems later — nothing here hard-codes them.
	 */
	UFUNCTION(BlueprintCallable, Category = "Rider")
	void SetLocomotionContext(FGameplayTag NewContext);

	/**
	 * First-person mesh override (arms/weapon overlay). Returns null in M1;
	 * the art milestone supplies the asset via Blueprint subclass. The hook
	 * exists so body-awareness presentation is an asset concern, not code.
	 */
	UFUNCTION(BlueprintPure, Category = "Rider")
	virtual USkeletalMeshComponent* GetFirstPersonMesh() const { return nullptr; }

	/** Broadcast when the locomotion context changes. */
	UPROPERTY(BlueprintAssignable, Category = "Rider")
	FDBLocomotionContextChangedSignature OnLocomotionContextChanged;

	// --- Configuration ---

	/** Input bindings source; falls back to the game mode's default. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UDBInputConfig> InputConfig;

	/** Fallback appearance (game mode can override). See DBRiderAppearanceDefinition. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	TObjectPtr<UDBRiderAppearanceDefinition> DefaultAppearance;

	/** Look sensitivity multiplier (mouse/gamepad sensitivity lives in the input assets). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	float LookSensitivity = 1.f;

	// --- ACharacter ---

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// --- IDBPerspectiveAware ---

	virtual bool ShouldHideBodyInFirstPerson_Implementation() const override { return true; }

protected:
	/** Resolve the input config (character > game mode) and the appearance, and apply them. */
	UDBInputConfig* ResolveInputConfig() const;
	void ResolveAndApplyAppearance();
	void BindCameraDirector();

	/** React to perspective changes: hide/show the full-body mesh. */
	UFUNCTION()
	void HandlePerspectiveChanged(EDBPerspective Perspective);

	// --- Input handlers (bound by identity from UDBInputConfig) ---

	void OnMoveTriggered(const FInputActionValue& Value);
	void OnMoveCompleted(const FInputActionValue& Value);
	void OnLookTriggered(const FInputActionValue& Value);
	void OnJumpStarted(const FInputActionValue& Value);
	void OnJumpCompleted(const FInputActionValue& Value);
	void OnSprintStarted(const FInputActionValue& Value);
	void OnSprintCompleted(const FInputActionValue& Value);
	void OnTogglePerspective(const FInputActionValue& Value);

protected:
	/** Cached movement input, consumed in Tick (cleared on Completed). */
	FVector2D MoveInput = FVector2D::ZeroVector;

	UPROPERTY(Transient)
	TObjectPtr<UDBRiderAppearanceDefinition> ActiveAppearance = nullptr;

	ERiderSex RiderSex = ERiderSex::Male;
	FGameplayTag LocomotionContext;
	bool bAppearanceApplied = false;
};
