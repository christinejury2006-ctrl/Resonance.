#include "Characters/DBDragonCharacter.h"
#include "Characters/DBDragonEmotionComponent.h"
#include "Characters/DBBondComponent.h"
#include "Characters/DBMindLinkComponent.h"
#include "Characters/DBDragonInteractionComponent.h"
#include "Characters/DBDragonVisualComponent.h"
#include "AI/DBDragonAIController.h"
#include "Dragonbound.h"
ADBDragonCharacter::ADBDragonCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 PrimaryActorTick.bCanEverTick = true;
 EmotionComponent = CreateDefaultSubobject<UDBDragonEmotionComponent>(TEXT("DragonEmotion"));
 BondComponent = CreateDefaultSubobject<UDBBondComponent>(TEXT("DragonBond"));
 MindLinkComponent = CreateDefaultSubobject<UDBMindLinkComponent>(TEXT("DragonMindLink"));
 InteractionComponent = CreateDefaultSubobject<UDBDragonInteractionComponent>(TEXT("DragonInteraction"));
 VisualComponent = CreateDefaultSubobject<UDBDragonVisualComponent>(TEXT("DragonVisual"));
 AIControllerClass = ADBDragonAIController::StaticClass();
 AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
 EmotionComponent->SetMood(EDBDragonMood::Neutral, 0.25f);
}
void ADBDragonCharacter::BeginPlay(){ Super::BeginPlay(); EmotionComponent->OnMoodChanged.AddDynamic(this, &ADBDragonCharacter::HandleMoodChanged); HandleMoodChanged(EDBDragonMood::Neutral, EmotionComponent->GetMood()); UE_LOG(LogDragonbound, Log, TEXT("Dragon '%s' entered play: bond stage Awakening."), *GetName()); }
FText ADBDragonCharacter::GetInteractionPrompt_Implementation() const { return NSLOCTEXT("DBInteraction", "DragonCallPrompt", "Call to the dragon"); }
void ADBDragonCharacter::HandleMoodChanged(EDBDragonMood PreviousMood, EDBDragonMood NewMood)
{
	if(!VisualComponent) return;
	EDBDragonVisualState VisualState = EDBDragonVisualState::Calm;
	switch(NewMood)
	{
	case EDBDragonMood::Curious:
	case EDBDragonMood::Excited: VisualState = EDBDragonVisualState::Alert; break;
	case EDBDragonMood::Comforted: VisualState = EDBDragonVisualState::Comforted; break;
	case EDBDragonMood::Fearful:
	case EDBDragonMood::Distressed: VisualState = EDBDragonVisualState::Distressed; break;
	case EDBDragonMood::Protective: VisualState = EDBDragonVisualState::Protective; break;
	default: break;
	}
	VisualComponent->SetVisualState(VisualState);
}
