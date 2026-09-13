// DragonboundCamera — camera mode implementation.

#include "DBCameraMode.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Pawn.h"

EDBPerspective GetOppositePerspective(EDBPerspective Perspective)
{
	return Perspective == EDBPerspective::ThirdPerson ? EDBPerspective::FirstPerson : EDBPerspective::ThirdPerson;
}

FDBPOV FDBPOV::Blend(const FDBPOV& A, const FDBPOV& B, float Alpha)
{
	Alpha = FMath::Clamp(Alpha, 0.f, 1.f);
	FDBPOV Result;
	Result.Location = FMath::Lerp(A.Location, B.Location, Alpha);
	Result.Rotation = FQuat::Slerp(A.Rotation.Quaternion(), B.Rotation.Quaternion(), Alpha).Rotator();
	Result.FOV = FMath::Lerp(A.FOV, B.FOV, Alpha);
	return Result;
}

UDBCameraMode::UDBCameraMode()
{
}

FDBPOV UDBCameraMode::ComputePOV(const FDBPOVContext& Context) const
{
	const AActor* ViewTarget = Context.ViewTarget;
	const FVector Pivot = GetPivotLocation(Context);
	return FDBPOV(Pivot, Context.ControlRotation, FieldOfView);
}

FVector UDBCameraMode::GetPivotLocation(const FDBPOVContext& Context) const
{
	if (!Context.ViewTarget)
	{
		return FVector::ZeroVector;
	}
	return Context.ViewTarget->GetActorLocation() + PivotOffset;
}

UDBCameraMode_ThirdPerson::UDBCameraMode_ThirdPerson()
{
	FieldOfView = 90.f;
	PitchMin = -80.f;
	PitchMax = 80.f;
}

FDBPOV UDBCameraMode_ThirdPerson::ComputePOV(const FDBPOVContext& Context) const
{
	const FVector Pivot = GetPivotLocation(Context);
	const FRotator DesiredRotation = Context.ControlRotation;

	FVector CameraLocation = Pivot - DesiredRotation.Vector() * ArmLength;
	if (!FMath::IsNearlyZero(ShoulderOffset))
	{
		CameraLocation += FRotationMatrix(DesiredRotation).GetUnitAxis(EAxis::Y) * ShoulderOffset;
	}

	return FDBPOV(CameraLocation, DesiredRotation, FieldOfView);
}

UDBCameraMode_FirstPerson::UDBCameraMode_FirstPerson()
{
	FieldOfView = 100.f;
	PivotOffset = FVector::ZeroVector;
}

FDBPOV UDBCameraMode_FirstPerson::ComputePOV(const FDBPOVContext& Context) const
{
	const AActor* ViewTarget = Context.ViewTarget;
	FVector EyeLocation = ViewTarget ? ViewTarget->GetActorLocation() + FVector(0.f, 0.f, 70.f) : FVector::ZeroVector;

	if (const APawn* Pawn = Cast<APawn>(ViewTarget))
	{
		bool bFoundEye = false;

		if (const USkeletalMeshComponent* Mesh = Pawn->FindComponentByClass<USkeletalMeshComponent>())
		{
			if (Mesh->DoesSocketExist(EyeSocketName))
			{
				EyeLocation = Mesh->GetSocketLocation(EyeSocketName);
				bFoundEye = true;
			}
		}

		if (!bFoundEye)
		{
			// No skeletal mesh (or no eye socket) yet: derive eye height from the capsule.
			const UCapsuleComponent* Capsule = Pawn->FindComponentByClass<UCapsuleComponent>();
			const float HalfHeight = Capsule ? Capsule->GetScaledCapsuleHalfHeight() : 88.f;
			EyeLocation = Pawn->GetActorLocation() + FVector(0.f, 0.f, HalfHeight - EyeFallbackInset);
		}
	}

	EyeLocation += EyeOffset;

	FRotator Rotation = Context.ControlRotation;
	Rotation.Roll = 0.f; // First-person view stays level; no camera roll on the ground.

	return FDBPOV(EyeLocation, Rotation, FieldOfView);
}
