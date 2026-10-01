#include "AI/DBDragonAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Characters/DBDragonCharacter.h"
#include "Characters/DBDragonEmotionComponent.h"
#include "Dragonbound.h"

ADBDDragonAIController::ADBDDragonAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("DragonPerception"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));

	SightConfig->SightRadius = 1800.f;
	SightConfig->LoseSightRadius = 2200.f;
	SightConfig->PeripheralVisionAngleDegrees = 100.f;
	SightConfig->SetMaxAge(3.f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	HearingConfig->HearingRange = 1400.f;
	HearingConfig->SetMaxAge(2.f);

	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->ConfigureSense(*HearingConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ADBDDragonAIController::HandlePerceptionUpdated);
}

void ADBDragonAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ADBDragonAIController::HandlePerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	ADBDDragonCharacter* Dragon = Cast<ADBDDragonCharacter>(GetPawn());
	if (!Dragon || !Actor)
	{
		return;
	}

	if (!Stimulus.WasSuccessfullySensed())
	{
		return;
	}

	// M2 first pass: seeing/hearing a new actor creates curiosity. The eventual
	// StateTree consumes this state and decides whether to approach, observe,
	// follow, or protect.
	if (UDBDragonEmotionComponent* Emotion = Dragon->GetEmotionComponent())
	{
		if (Actor != Dragon)
		{
			Emotion->SetMood(EDBDragonMood::Curious, 0.65f);
		}
	}

	UE_LOG(LogDragonbound, Verbose, TEXT("Dragon '%s' perceived '%s'."), *Dragon->GetName(), *Actor->GetName());
}