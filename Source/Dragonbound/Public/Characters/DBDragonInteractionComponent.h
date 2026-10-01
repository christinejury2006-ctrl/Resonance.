// Dragonbound — M2 Rider/dragon interaction loop.
//
// These are gameplay verbs, not UI-only events. Each action changes emotion,
// bond state and/or memory so later AI can react to the relationship.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DBDragonInteractionComponent.generated.h"

class AActor;

UENUM(BlueprintType)
enum class EDBDragonInteraction : uint8
{
	Call,
	Feed,
	Soothe,
	Protect
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDBDragonInteractionSignature, EDBDragonInteraction, Interaction, AActor*, Instigator);

UCLASS(ClassGroup=(Dragonbound), meta=(BlueprintSpawnableComponent))
class DRAGONBOUND_API UDBDragonInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDBDragonInteractionComponent();

	UFUNCTION(BlueprintCallable, Category="Dragon|Interaction")
	bool PerformInteraction(EDBDragonInteraction Interaction, AActor* Instigator);

	UFUNCTION(BlueprintPure, Category="Dragon|Interaction")
	bool IsInteractionAvailable(EDBDragonInteraction Interaction) const;

	UPROPERTY(BlueprintAssignable, Category="Dragon|Interaction")
	FDBDragonInteractionSignature OnInteraction;

protected:
	void ApplyInteraction(EDBDragonInteraction Interaction, AActor* Instigator);
};