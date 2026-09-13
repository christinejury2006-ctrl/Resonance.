// Dragonbound — Rider movement component.
//
// The Rider's ground locomotion: jog/sprint speeds, acceleration/braking,
// camera-relative movement, jump, and tuned air control. Mounted and Flying
// states are NOT implemented here — they arrive as separate movement modes
// on the dragon systems (M6), driven by the DB.Locomotion.* context tags
// (see ADBRiderCharacter::SetLocomotionContext and ADR-0002).

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DBRiderMovementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDBSprintChangedSignature, bool, bIsSprinting);

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class DRAGONBOUND_API UDBRiderMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UDBRiderMovementComponent(const FObjectInitializer& ObjectInitializer);

	/** Toggle sprint. Speed changes apply on the ground; the intent persists while airborne. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetSprinting(bool bInSprinting);

	UFUNCTION(BlueprintPure, Category = "Movement")
	bool IsSprinting() const { return bSprinting; }

	// --- Tuning values (M1 defaults below; Blueprint subclasses may retune) ---

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "1.0"))
	float JogSpeed = 420.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = "1.0"))
	float SprintSpeed = 680.f;

	/** Broadcast when sprint state changes (audio, camera, and AI hooks later). */
	UPROPERTY(BlueprintAssignable, Category = "Movement")
	FDBSprintChangedSignature OnSprintChanged;

private:
	bool bSprinting = false;
};
