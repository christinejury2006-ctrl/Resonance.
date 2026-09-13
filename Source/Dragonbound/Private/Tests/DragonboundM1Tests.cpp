// Dragonbound — M1 automation tests.
//
// World-less unit checks for the M1 foundation. Run in-editor:
//   Session Frontend > Automation > Dragonbound.M1.*
// or from the command line:
//   UnrealEditor-Cmd.exe <Project> -ExecCmds="Automation RunTests Dragonbound.M1; Quit"
//
// These intentionally avoid level/asset dependencies so they pass on a
// clean checkout before any content is authored (Docs/EDITOR_SETUP.md).

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "DBCameraMode.h"
#include "DBNativeGameplayTags.h"
#include "Characters/DBRiderCharacter.h"
#include "Characters/DBRiderMovementComponent.h"
#include "Game/DBRiderGameMode.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDBM1_POVBlendTest, "Dragonbound.M1.Camera.POVBlend", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FDBM1_POVBlendTest::RunTest(const FString& Parameters)
{
	const FDBPOV A(FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f), 90.f);
	const FDBPOV B(FVector(100.f, 0.f, 0.f), FRotator(0.f, 90.f, 0.f), 100.f);

	const FDBPOV Mid = FDBPOV::Blend(A, B, 0.5f);

	TestEqual(TEXT("Location lerps"), Mid.Location.X, 50.f, 0.01f);
	TestEqual(TEXT("FOV lerps"), Mid.FOV, 95.f, 0.01f);
	TestEqual(TEXT("Rotation slerps"), Mid.Rotation.Yaw, 45.f, 0.1f);

	const FDBPOV ClampedLow = FDBPOV::Blend(A, B, -0.5f);
	const FDBPOV ClampedHigh = FDBPOV::Blend(A, B, 1.5f);
	TestEqual(TEXT("Alpha clamps low"), ClampedLow.Location.X, A.Location.X, 0.01f);
	TestEqual(TEXT("Alpha clamps high"), ClampedHigh.Location.X, B.Location.X, 0.01f);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDBM1_PerspectiveToggleTest, "Dragonbound.M1.Camera.PerspectiveToggle", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FDBM1_PerspectiveToggleTest::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Opposite of ThirdPerson is FirstPerson"), GetOppositePerspective(EDBPerspective::ThirdPerson), EDBPerspective::FirstPerson);
	TestEqual(TEXT("Opposite of FirstPerson is ThirdPerson"), GetOppositePerspective(EDBPerspective::FirstPerson), EDBPerspective::ThirdPerson);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDBM1_MovementDefaultsTest, "Dragonbound.M1.Movement.Defaults", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FDBM1_MovementDefaultsTest::RunTest(const FString& Parameters)
{
	UDBRiderMovementComponent* Movement = NewObject<UDBRiderMovementComponent>();
	TestNotNull(TEXT("Movement component created"), Movement);

	TestEqual(TEXT("Default speed is jog"), Movement->MaxWalkSpeed, Movement->JogSpeed);

	Movement->SetSprinting(true);
	TestTrue(TEXT("Sprint state set"), Movement->IsSprinting());
	TestEqual(TEXT("Sprint speed applied"), Movement->MaxWalkSpeed, Movement->SprintSpeed);

	Movement->SetSprinting(false);
	TestFalse(TEXT("Sprint state cleared"), Movement->IsSprinting());
	TestEqual(TEXT("Jog speed restored"), Movement->MaxWalkSpeed, Movement->JogSpeed);

	TestTrue(TEXT("Body orients to movement"), Movement->bOrientRotationToMovement);
	TestTrue(TEXT("Air control is tuned"), Movement->AirControl > 0.f);
	TestTrue(TEXT("Jump velocity is tuned"), Movement->JumpZVelocity > 0.f);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDBM1_CharacterDefaultsTest, "Dragonbound.M1.Character.Defaults", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FDBM1_CharacterDefaultsTest::RunTest(const FString& Parameters)
{
	const ADBRiderCharacter* CharacterCDO = GetDefault<ADBRiderCharacter>();
	TestNotNull(TEXT("Rider character CDO"), CharacterCDO);

	TestTrue(TEXT("Character uses the Rider movement component"),
		Cast<UDBRiderMovementComponent>(CharacterCDO->GetCharacterMovement()) != nullptr);

	const ADBRiderGameMode* GameModeCDO = GetDefault<ADBRiderGameMode>();
	TestNotNull(TEXT("Rider game mode CDO"), GameModeCDO);
	TestEqual(TEXT("Game mode spawns the Rider"), GameModeCDO->DefaultPawnClass.Get(), ADBRiderCharacter::StaticClass());

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDBM1_NativeTagsTest, "Dragonbound.M1.Tags.Registered", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FDBM1_NativeTagsTest::RunTest(const FString& Parameters)
{
	TestTrue(TEXT("Locomotion.OnFoot is valid"), DBGameplayTags::Locomotion_OnFoot.IsValid());
	TestEqual(TEXT("Locomotion.OnFoot name"), DBGameplayTags::Locomotion_OnFoot.GetTagName(), FName(TEXT("DB.Locomotion.OnFoot")));

	TestTrue(TEXT("Locomotion.Mounted is reserved"), DBGameplayTags::Locomotion_Mounted.IsValid());
	TestTrue(TEXT("Locomotion.Flying is reserved"), DBGameplayTags::Locomotion_Flying.IsValid());

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
