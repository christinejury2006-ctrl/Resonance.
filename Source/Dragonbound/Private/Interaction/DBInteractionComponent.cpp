// Dragonbound — interaction trace implementation.

#include "Interaction/DBInteractionComponent.h"
#include "CollisionQueryParams.h"
#include "CollisionShape.h"
#include "DBCameraDirectorComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/DBInteractable.h"

#if ENABLE_DRAW_DEBUG
#include "DrawDebugHelpers.h"
#endif

UDBInteractionComponent::UDBInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	// Interaction does not need per-frame traces; 10 Hz keeps it cheap.
	SetComponentTickInterval(0.1f);
}

APlayerController* UDBInteractionComponent::GetPlayerController() const
{
	return Cast<APlayerController>(GetOwner());
}

AActor* UDBInteractionComponent::FindInteractionTarget() const
{
	const APlayerController* PlayerController = GetPlayerController();
	if (!PlayerController)
	{
		return nullptr;
	}

	FVector Origin = FVector::ZeroVector;
	FRotator Orientation = FRotator::ZeroRotator;

	// Trace from the rendered view (the camera director's POV is authoritative
	// in both perspectives).
	if (const UDBCameraDirectorComponent* Director = PlayerController->FindComponentByClass<UDBCameraDirectorComponent>())
	{
		Origin = Director->GetLatestPOV().Location;
		Orientation = Director->GetLatestPOV().Rotation;
	}
	else if (PlayerController->PlayerCameraManager)
	{
		Origin = PlayerController->PlayerCameraManager->GetCameraLocation();
		Orientation = PlayerController->PlayerCameraManager->GetCameraRotation();
	}

	const FVector End = Origin + Orientation.Vector() * InteractionDistance;

	FCollisionQueryParams Params(FName(TEXT("DBInteractionTrace")), false, PlayerController->GetPawn());
	Params.AddIgnoredActor(PlayerController);

	FHitResult Hit;
	if (GetWorld()->SweepSingleByChannel(Hit, Origin, End, FQuat::Identity, TraceChannel,
		FCollisionShape::MakeSphere(TraceRadius), Params))
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor->Implements<UDBInteractable>())
			{
				return HitActor;
			}
		}
	}

	return nullptr;
}

void UDBInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* NewTarget = FindInteractionTarget();
	if (NewTarget != CurrentTarget)
	{
		CurrentTarget = NewTarget;
		OnInteractionTargetChanged.Broadcast(CurrentTarget);
	}

#if ENABLE_DRAW_DEBUG
	if (bDrawDebugTrace)
	{
		if (const APlayerController* PlayerController = GetPlayerController())
		{
			const FVector Start = PlayerController->PlayerCameraManager ? PlayerController->PlayerCameraManager->GetCameraLocation() : FVector::ZeroVector;
			const FVector End = Start + (PlayerController->PlayerCameraManager ? PlayerController->PlayerCameraManager->GetCameraRotation() : FRotator::ZeroRotator).Vector() * InteractionDistance;
			DrawDebugLine(GetWorld(), Start, End, CurrentTarget ? FColor::Green : FColor::Yellow, false, 0.2f, 0, 1.f);
		}
	}
#endif
}
