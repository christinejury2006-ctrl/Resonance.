// Dragonbound — M2 dragon AI controller.
//
// Perception is the sensing layer. StateTree remains the decision layer and
// will be authored as an editor asset once the perception contract is in
// place; this controller must not become a second behaviour-tree system.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "DBDragonAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;

UCLASS()
class DRAGONBOUND_API ADBDragonAIController : public AAIController
{
	GENERATED_BODY()

public:
	ADBDDragonAIController();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dragon AI")
	TObjectPtr<UAIPerceptionComponent> PerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dragon AI")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dragon AI")
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;

	UFUNCTION()
	void HandlePerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};