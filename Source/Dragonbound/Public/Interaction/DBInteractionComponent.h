// Dragonbound — interaction trace.
//
// A cheap camera-forward trace (10 Hz) that exposes the current
// IDBInteractable target. M2's dragon interaction consumes the same
// component: interact-with-dragon is an ordinary interaction target, not a
// special-case system.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/EngineTypes.h"
#include "DBInteractionComponent.generated.h"

class UDBCameraDirectorComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDBInteractionTargetChangedSignature, AActor*, NewTarget);

UCLASS(ClassGroup = (Interaction), Blueprintable, meta = (BlueprintSpawnableComponent))
class DRAGONBOUND_API UDBInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDBInteractionComponent();

	UFUNCTION(BlueprintPure, Category = "Interaction")
	AActor* GetCurrentInteractionTarget() const { return CurrentTarget; }

	/** Broadcast when the trace's interactable target changes (new target or lost target). */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FDBInteractionTargetChangedSignature OnInteractionTargetChanged;

	// --- Config ---

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction", meta = (ClampMin = "1.0"))
	float InteractionDistance = 300.f;

	/** Sphere-trace radius for lenient targeting. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction", meta = (ClampMin = "0.0"))
	float TraceRadius = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction|Debug")
	bool bDrawDebugTrace = false;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	AActor* FindInteractionTarget() const;
	APlayerController* GetPlayerController() const;

	UPROPERTY(Transient)
	TObjectPtr<AActor> CurrentTarget = nullptr;
};
