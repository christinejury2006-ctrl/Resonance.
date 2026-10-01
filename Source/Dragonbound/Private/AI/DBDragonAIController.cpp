#include "AI/DBDragonAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Components/StateTreeAIComponent.h"
#include "Characters/DBDragonCharacter.h"
#include "Characters/DBDragonEmotionComponent.h"
#include "Characters/DBDragonVisualComponent.h"
#include "Characters/DBBondComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/Pawn.h"
#include "Dragonbound.h"

namespace
{
	constexpr float M2FollowBondDepth = 10.f;
}

ADBDragonAIController::ADBDragonAIController(){ PrimaryActorTick.bCanEverTick=true; PerceptionComponent=CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("DragonPerception")); SightConfig=CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig")); HearingConfig=CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig")); StateTreeComponent=CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("DragonStateTree")); StateTreeComponent->SetStartLogicAutomatically(false); SightConfig->SightRadius=1800.f; SightConfig->LoseSightRadius=2200.f; SightConfig->PeripheralVisionAngleDegrees=100.f; SightConfig->SetMaxAge(3.f); SightConfig->DetectionByAffiliation.bDetectEnemies=true; SightConfig->DetectionByAffiliation.bDetectNeutrals=true; SightConfig->DetectionByAffiliation.bDetectFriendlies=true; HearingConfig->HearingRange=1400.f; HearingConfig->SetMaxAge(2.f); PerceptionComponent->ConfigureSense(*SightConfig); PerceptionComponent->ConfigureSense(*HearingConfig); PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation()); PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this,&ADBDragonAIController::HandlePerceptionUpdated); }
void ADBDragonAIController::BeginPlay(){ Super::BeginPlay(); SetAIState(EDBDragonAIState::Idle); if(ADBDragonCharacter* Dragon=Cast<ADBDragonCharacter>(GetPawn())){if(UDBDragonVisualComponent* Visual=Dragon->GetVisualComponent()){switch(Visual->GetGrowthStage()){case EDBDragonGrowthStage::Hatchling:SightConfig->SightRadius=700.f;SightConfig->LoseSightRadius=850.f;HearingConfig->HearingRange=650.f;break;case EDBDragonGrowthStage::Juvenile:SightConfig->SightRadius=1200.f;SightConfig->LoseSightRadius=1500.f;HearingConfig->HearingRange=950.f;break;case EDBDragonGrowthStage::YoungAdult:SightConfig->SightRadius=1800.f;SightConfig->LoseSightRadius=2200.f;HearingConfig->HearingRange=1400.f;break;case EDBDragonGrowthStage::Adult:SightConfig->SightRadius=2400.f;SightConfig->LoseSightRadius=3000.f;HearingConfig->HearingRange=1900.f;break;}}} if(StateTreeComponent&&DragonStateTree){StateTreeComponent->SetStateTree(DragonStateTree);StateTreeComponent->StartLogic();} }
void ADBDragonAIController::Tick(float DeltaSeconds){ Super::Tick(DeltaSeconds); if(DragonStateTree)return; DecisionTimer-=DeltaSeconds; if(DecisionTimer<=0.f){DecisionTimer=DecisionInterval;EvaluateDecision();} }
void ADBDragonAIController::HandlePerceptionUpdated(AActor* Actor, FAIStimulus Stimulus){ ADBDragonCharacter* Dragon=Cast<ADBDragonCharacter>(GetPawn()); if(!Dragon||!Actor||Actor==Dragon)return; if(!Stimulus.WasSuccessfullySensed()){if(Actor==FocusActor){FocusActor=nullptr;SetAIState(EDBDragonAIState::Idle);}return;} if(APawn* PerceivedPawn=Cast<APawn>(Actor)){FocusActor=PerceivedPawn;SetAIState(EDBDragonAIState::Curious);} if(UDBDragonEmotionComponent* Emotion=Dragon->GetEmotionComponent())Emotion->SetMood(EDBDragonMood::Curious,0.65f); UE_LOG(LogDragonbound,Verbose,TEXT("Dragon '%s' perceived '%s'."),*Dragon->GetName(),*Actor->GetName()); }
void ADBDragonAIController::EvaluateDecision(){ ADBDragonCharacter* Dragon=Cast<ADBDragonCharacter>(GetPawn()); if(!Dragon)return; if(!FocusActor){SetAIState(EDBDragonAIState::Idle);StopMovement();return;} const float Distance=FVector::Dist(GetPawn()->GetActorLocation(),FocusActor->GetActorLocation()); const float BondDepth=Dragon->GetBondComponent()?Dragon->GetBondComponent()->GetBondDepth():0.f; if(BondDepth>=M2FollowBondDepth&&Distance<=1200.f){SetAIState(EDBDragonAIState::Follow);MoveTowardFocus();return;} if(Distance<=900.f){SetAIState(EDBDragonAIState::Curious);StopMovement();return;} SetAIState(EDBDragonAIState::React);MoveTowardFocus(); }
void ADBDragonAIController::SetAIState(EDBDragonAIState NewState){if(CurrentState!=NewState)CurrentState=NewState;}
void ADBDragonAIController::MoveTowardFocus(){if(!FocusActor)return; FAIMoveRequest Request(FocusActor);Request.SetAcceptanceRadius(FollowDistance);Request.SetUsePathfinding(true);MoveTo(Request);}
void ADBDragonAIController::RespondToCall(AActor* Caller){if(!Caller)return;FocusActor=Caller;SetAIState(EDBDragonAIState::Curious);MoveTowardFocus();}
void ADBDragonAIController::RespondToProtect(AActor* Protector){if(!Protector)return;FocusActor=Protector;SetAIState(EDBDragonAIState::Protect);StopMovement();}
