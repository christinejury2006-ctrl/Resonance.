// Dragonbound — M2 automation tests.
//
// World-less checks for the dragon companion/bond foundation. These validate
// the gameplay state contract without requiring authored meshes, StateTrees,
// animation assets, or a level.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "Characters/DBDragonCharacter.h"
#include "Input/DBControlRouterComponent.h"
#include "DBNativeGameplayTags.h"
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
	const ADBDragonCharacter* CharacterCDO = GetDefault<ADBDragonCharacter>();
	TestNotNull(TEXT("Dragon character CDO"), CharacterCDO);
	UDBDragonInteractionComponent* Interaction = CharacterCDO->GetInteractionComponent();
	TestNotNull(TEXT("Interaction component exists"), Interaction);
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



IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDragonboundM2ControlRouter, "Dragonbound.M2.Controls.LocomotionGating", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FDragonboundM2ControlRouter::RunTest(const FString& Parameters)
{
	UDBControlRouterComponent* Router = NewObject<UDBControlRouterComponent>();
	TestNotNull(TEXT("Control router is created"), Router);
	if (!Router)
	{
		return false;
	}

	TestFalse(TEXT("None is never executable"), Router->CanExecuteCommand(EDBControlCommand::None));

	Router->SetLocomotionContext(DBGameplayTags::Locomotion_OnFoot);
	TestTrue(TEXT("Mount is available on foot"), Router->CanExecuteCommand(EDBControlCommand::Mount));
	TestTrue(TEXT("Call is available on foot"), Router->CanExecuteCommand(EDBControlCommand::Call));
	TestTrue(TEXT("Feed is available on foot"), Router->CanExecuteCommand(EDBControlCommand::Feed));
	TestTrue(TEXT("Soothe is available on foot"), Router->CanExecuteCommand(EDBControlCommand::Soothe));
	TestTrue(TEXT("Protect is available on foot"), Router->CanExecuteCommand(EDBControlCommand::Protect));
	TestFalse(TEXT("Dismount is unavailable on foot"), Router->CanExecuteCommand(EDBControlCommand::Dismount));
	TestFalse(TEXT("Primary ability is unavailable on foot"), Router->CanExecuteCommand(EDBControlCommand::PrimaryAbility));

	Router->SetLocomotionContext(DBGameplayTags::Locomotion_Mounted);
	TestFalse(TEXT("Mount is unavailable while mounted"), Router->CanExecuteCommand(EDBControlCommand::Mount));
	TestTrue(TEXT("Dismount is available while mounted"), Router->CanExecuteCommand(EDBControlCommand::Dismount));
	TestTrue(TEXT("Primary ability is available while mounted"), Router->CanExecuteCommand(EDBControlCommand::PrimaryAbility));
	TestFalse(TEXT("Call is unavailable while mounted"), Router->CanExecuteCommand(EDBControlCommand::Call));

	Router->SetLocomotionContext(DBGameplayTags::Locomotion_Flying);
	TestFalse(TEXT("Mount is unavailable while flying"), Router->CanExecuteCommand(EDBControlCommand::Mount));
	TestTrue(TEXT("Dismount is available while flying"), Router->CanExecuteCommand(EDBControlCommand::Dismount));
	TestTrue(TEXT("Secondary ability is available while flying"), Router->CanExecuteCommand(EDBControlCommand::SecondaryAbility));

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS




IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDBM2_BondMemoryProgressionTest, "Dragonbound.M2.Bond.MemoryProgression", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FDBM2_BondMemoryProgressionTest::RunTest(const FString& Parameters)
{
	UDBBondComponent* Bond = NewObject<UDBBondComponent>();
	TestNotNull(TEXT("Bond component is created"), Bond);
	if (!Bond)
	{
		return false;
	}

	FDBondEvent Event;
	Event.EventId = FName(TEXT("Test.Soothe"));
	Event.Valence = 5.f;
	Event.bMemoryFlagged = true;

	Bond->RecordEvent(Event);

	TestEqual(TEXT("Memory records the flagged event"), Bond->GetMemory().Num(), 1);
	TestEqual(TEXT("Bond depth gains the event valence"), Bond->GetBondDepth(), 5.f);
	TestEqual(TEXT("Trust gains half the event valence"), Bond->GetTrust(), 2.5f);
	TestEqual(TEXT("Stage remains story-gated below threshold"), Bond->GetBondStage(), EDBBondStage::Awakening);

	Bond->RecordEvent(Event);
	TestEqual(TEXT("Second event is retained"), Bond->GetMemory().Num(), 2);
	TestEqual(TEXT("Bond reaches the M2 Feeling threshold"), Bond->GetBondDepth(), 10.f);

	Bond->AdvanceStage(EDBBondStage::Feeling);
	TestEqual(TEXT("Feeling stage can be advanced explicitly"), Bond->GetBondStage(), EDBBondStage::Feeling);

	return true;
}

