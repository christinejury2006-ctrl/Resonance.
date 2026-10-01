// Dragonbound — M2 dragon decision controller.
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "StateTree.h"
#include "Perception/AIPerceptionTypes.h"
#include "DBDragonAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UStateTreeAIComponent;

UENUM(BlueprintType)
enum class EDBDragonAIState : uint8
{
	Idle,
	Curious,
	Follow,
	Protect,
	React
};

UCLASS()
class DRAGONBOUND_API ADBDragonAIController : public AAIController
{
	GENERATED_BODY()

public:
	ADBDragonAIController();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dragon AI")
	TObjectPtr<UAIPerceptionComponent> PerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dragon AI")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dragon AI")
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dragon AI")
	TObjectPtr<UStateTreeAIComponent> StateTreeComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dragon AI")
	TObjectPtr<UStateTree> DragonStateTree;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Dragon AI")
	EDBDragonAIState CurrentState = EDBDragonAIState::Idle;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Dragon AI")
	TObjectPtr<AActor> FocusActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dragon AI")
	float FollowDistance = 350.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dragon AI")
	float DecisionInterval = 0.35f;

	float DecisionTimer = 0.f;

	UFUNCTION()
	void HandlePerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	void EvaluateDecision();
	void SetAIState(EDBDragonAIState NewState);
	void MoveTowardFocus();
};