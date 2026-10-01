#include "Characters/DBDragonCharacter.h"
#include "Characters/DBDragonEmotionComponent.h"
#include "AI/DBDragonAIController.h"
#include "Characters/DBBondComponent.h"
#include "Characters/DBMindLinkComponent.h"
#include "Dragonbound.h"

ADBDragonCharacter::ADBDDragonCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	EmotionComponent = CreateDefaultSubobject<UDBDragonEmotionComponent>(TEXT("DragonEmotion"));
	BondComponent = CreateDefaultSubobject<UDBBondComponent>(TEXT("DragonBond"));
	MindLinkComponent = CreateDefaultSubobject<UDBMindLinkComponent>(TEXT("DragonMindLink"));

	AIControllerClass = ADBDragonAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// M2 foundation starts with a neutral, low-intensity emotional state.
	EmotionComponent->SetMood(EDBDragonMood::Neutral, 0.25f);
}

void ADBDragonCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogDragonbound, Log, TEXT("Dragon '%s' entered play: bond stage Awakening."), *GetName());
}