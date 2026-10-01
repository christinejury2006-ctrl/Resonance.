#include "Characters/DBDragonCharacter.h"
#include "Characters/DBDragonEmotionComponent.h"
#include "Characters/DBBondComponent.h"
#include "Characters/DBMindLinkComponent.h"
#include "Characters/DBDragonInteractionComponent.h"
#include "AI/DBDragonAIController.h"
#include "Dragonbound.h"
ADBDragonCharacter::ADBDragonCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 PrimaryActorTick.bCanEverTick = true;
 EmotionComponent = CreateDefaultSubobject<UDBDragonEmotionComponent>(TEXT("DragonEmotion"));
 BondComponent = CreateDefaultSubobject<UDBBondComponent>(TEXT("DragonBond"));
 MindLinkComponent = CreateDefaultSubobject<UDBMindLinkComponent>(TEXT("DragonMindLink"));
 InteractionComponent = CreateDefaultSubobject<UDBDragonInteractionComponent>(TEXT("DragonInteraction"));
 AIControllerClass = ADBDragonAIController::StaticClass();
 AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
 EmotionComponent->SetMood(EDBDragonMood::Neutral, 0.25f);
}
void ADBDragonCharacter::BeginPlay(){ Super::BeginPlay(); UE_LOG(LogDragonbound, Log, TEXT("Dragon '%s' entered play: bond stage Awakening."), *GetName()); }
FText ADBDragonCharacter::GetInteractionPrompt_Implementation() const { return NSLOCTEXT("DBInteraction", "DragonCallPrompt", "Call to the dragon"); }