// Dragonbound — M2 automation tests.
//
// World-less checks for the dragon companion/bond foundation. These validate
// the gameplay state contract without requiring authored meshes, StateTrees,
// animation assets, or a level.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "Characters/DBDragonCharacter.h"
#include "Characters/DBDragonEmotionComponent.h"
#include "Characters/DBDragonInteractionComponent.h"
#include "Characters/DBDragonVisualComponent.h"
#include "Characters/DBBondComponent.h"
#include "Characters/DBMindLinkComponent.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDBM2_DragonComponentDefaultsTest, "Dragonbound.M2.Dragon.ComponentDefaults", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FDBM2_DragonComponentDefaultsTest::RunTest(const FString& Parameters)
{
	const ADBDragonCharacter* CharacterCDO = GetDefault<ADBDragonCharacter>();
	TestNotNull(TEXT("Dragon character CDO"), CharacterCDO);
	TestNotNull(TEXT("Emotion component exists"), CharacterCDO->GetEmotionComponent());
	TestNotNull(TEXT("Bond component exists"), CharacterCDO->GetBondComponent());
	TestNotNull(TEXT("Mind-link component exists"), CharacterCDO->GetMindLinkComponent());
	TestNotNull(TEXT("Interaction component exists"), CharacterCDO->GetInteractionComponent());
	TestNotNull(TEXT("Visual component exists"), CharacterCDO->GetVisualComponent());

	TestEqual(TEXT("M2 starts at Awakening"), CharacterCDO->GetBondComponent()->GetBondStage(), EDBBondStage::Awakening);
	TestEqual(TEXT("M2 dragon starts neutral"), CharacterCDO->GetEmotionComponent()->GetMood(), EDBDragonMood::Neutral);
	TestEqual(TEXT("M2 dragon starts at juvenile stage"), CharacterCDO->GetVisualComponent()->GetGrowthStage(), EDBDragonGrowthStage::Juvenile);
	TestEqual(TEXT("M2 dragon starts visually calm"), CharacterCDO->GetVisualComponent()->GetVisualState(), EDBDragonVisualState::Calm);
	TestTrue(TEXT("Mind-link supports feeling payloads"), CharacterCDO->GetMindLinkComponent()->CanSendFeelingPayloads());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDBM2_VisualStateTest, "Dragonbound.M2.Dragon.VisualState", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FDBM2_VisualStateTest::RunTest(const FString& Parameters)
{
	UDBDragonVisualComponent* Visual = NewObject<UDBDragonVisualComponent>();
	TestNotNull(TEXT("Visual component created"), Visual);

	Visual->SetGrowthStage(EDBDragonGrowthStage::Hatchling);
	TestEqual(TEXT("Growth stage changes"), Visual->GetGrowthStage(), EDBDragonGrowthStage::Hatchling);

	Visual->SetVisualState(EDBDragonVisualState::Protective);
	TestEqual(TEXT("Visual state changes"), Visual->GetVisualState(), EDBDragonVisualState::Protective);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDBM2_InteractionAvailabilityTest, "Dragonbound.M2.Dragon.InteractionAvailability", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FDBM2_InteractionAvailabilityTest::RunTest(const FString& Parameters)
{
	UDBDragonInteractionComponent* Interaction = NewObject<UDBDragonInteractionComponent>();
	TestNotNull(TEXT("Interaction component created"), Interaction);
	TestTrue(TEXT("Call available"), Interaction->IsInteractionAvailable(EDBDragonInteraction::Call));
	TestTrue(TEXT("Feed available"), Interaction->IsInteractionAvailable(EDBDragonInteraction::Feed));
	TestTrue(TEXT("Soothe available"), Interaction->IsInteractionAvailable(EDBDragonInteraction::Soothe));
	TestTrue(TEXT("Protect available"), Interaction->IsInteractionAvailable(EDBDragonInteraction::Protect));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDBM2_MindLinkPayloadTest, "Dragonbound.M2.MindLink.FeelingPayload", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FDBM2_MindLinkPayloadTest::RunTest(const FString& Parameters)
{
	UDBMindLinkComponent* Link = NewObject<UDBMindLinkComponent>();
	TestNotNull(TEXT("Mind-link component created"), Link);

	FDBMindLinkPayload Payload;
	Payload.Type = EDBMindLinkPayloadType::Sensation;
	Payload.Cue = FName(TEXT("Warmth"));
	Payload.Intensity = 0.65f;
	Payload.Duration = 1.0f;

	Link->SendPayload(Payload);
	TestTrue(TEXT("Feeling payload remains enabled"), Link->CanSendFeelingPayloads());
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
