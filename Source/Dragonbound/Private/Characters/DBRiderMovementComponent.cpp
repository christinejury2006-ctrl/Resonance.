// Dragonbound — Rider movement component implementation.

#include "Characters/DBRiderMovementComponent.h"

UDBRiderMovementComponent::UDBRiderMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Ground locomotion (M1 tuning; retunable in Blueprint subclasses).
	MaxWalkSpeed = JogSpeed;
	MaxWalkSpeedCrouched = 200.f;
	MaxAcceleration = 2400.f;
	BrakingDecelerationWalking = 2500.f;
	GroundFriction = 6.f;

	// Rotation follows movement input; no controller-forced yaw.
	bOrientRotationToMovement = true;
	bUseControllerDesiredRotation = false;
	RotationRate = FRotator(0.f, 720.f, 0.f);

	// Air control: enough to steer a jump, not enough to feel floaty.
	AirControl = 0.4f;
	AirControlBoostMultiplier = 2.f;
	AirControlBoostVelocityThreshold = 20.f;

	// Weight: a committed jump with a clear arc.
	JumpZVelocity = 640.f;
	GravityScale = 1.f;
}

void UDBRiderMovementComponent::SetSprinting(bool bInSprinting)
{
	if (bSprinting == bInSprinting)
	{
		return;
	}

	bSprinting = bInSprinting;
	MaxWalkSpeed = bSprinting ? SprintSpeed : JogSpeed;
	OnSprintChanged.Broadcast(bSprinting);
}
