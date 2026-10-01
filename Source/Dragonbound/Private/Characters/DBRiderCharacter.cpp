// Dragonbound — Rider character implementation.

#include "Characters/DBRiderCharacter.h"
#include "Characters/DBRiderAppearanceDefinition.h"
#include "Characters/DBRiderMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DBCameraDirectorComponent.h"
#include "DBNativeGameplayTags.h"
#include "Dragonbound.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Game/DBRiderGameMode.h"
#include "Input/DBInputConfig.h"
#include "Player/DBRiderPlayerController.h"
#include "Interaction/DBInteractionComponent.h"
#include "Characters/DBDragonCharacter.h"
#include "Characters/DBDragonInteractionComponent.h"
#include "Characters/DBMindLinkReceiverComponent.h"
#include "Input/DBControlRouterComponent.h"

ADBRiderCharacter::ADBRiderCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UDBRiderMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	// The character never rotates from camera input; the camera is free
	// while the body follows movement direction.
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	MindLinkReceiver = CreateDefaultSubobject<UDBMindLinkReceiverComponent>(TEXT("MindLinkReceiver"));
}

UDBRiderMovementComponent* ADBRiderCharacter::GetRiderMovementComponent() const
{
	return Cast<UDBRiderMovementComponent>(GetCharacterMovement());
}

UDBInputConfig* ADBRiderCharacter::ResolveInputConfig() const
{
	if (InputConfig)
	{
		return InputConfig;
	}

	if (const ADBRiderGameMode* GameMode = GetWorld() ? Cast<ADBRiderGameMode>(GetWorld()->GetAuthGameMode()) : nullptr)
	{
		return GameMode->DefaultInputConfig;
	}

	return nullptr;
}

void ADBRiderCharacter::ResolveAndApplyAppearance()
{
	if (bAppearanceApplied)
	{
		return;
	}

	UDBRiderAppearanceDefinition* Definition = DefaultAppearance;
	if (!Definition)
	{
		if (const ADBRiderGameMode* GameMode = GetWorld() ? Cast<ADBRiderGameMode>(GetWorld()->GetAuthGameMode()) : nullptr)
		{
			Definition = GameMode->DefaultAppearanceDefinition;
		}
	}

	if (!Definition)
	{
		UE_LOG(LogDragonbound, Warning, TEXT("Rider '%s': no appearance definition configured (character class default or game mode). Placeholder avatar not applied."), *GetName());
		return;
	}

	ActiveAppearance = Definition;
	RiderSex = Definition->Sex;

	if (USkeletalMesh* BodyMesh = Definition->BodyMesh.LoadSynchronous())
	{
		GetMesh()->SetSkeletalMesh(BodyMesh);
	}
	if (UClass* AnimClass = Definition->AnimInstanceClass.LoadSynchronous())
	{
		GetMesh()->SetAnimInstanceClass(AnimClass);
	}
	GetCapsuleComponent()->SetCapsuleSize(Definition->CapsuleRadius, Definition->CapsuleHalfHeight);

	bAppearanceApplied = true;
	UE_LOG(LogDragonbound, Log, TEXT("Rider '%s': appearance applied (sex=%s)."), *GetName(), RiderSex == ERiderSex::Male ? TEXT("Male") : TEXT("Female"));
}

void ADBRiderCharacter::BeginPlay()
{
	Super::BeginPlay();

	ResolveAndApplyAppearance();

	// M1: the Rider always starts on foot. Mounted/Flying are set by the
	// dragon systems (M6+) through the same SetLocomotionContext path.
	SetLocomotionContext(DBGameplayTags::Locomotion_OnFoot);
}

void ADBRiderCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	BindCameraDirector();
}

void ADBRiderCharacter::BindCameraDirector()
{
	if (ADBRiderPlayerController* PlayerController = Cast<ADBRiderPlayerController>(GetController()))
	{
		if (UDBCameraDirectorComponent* Director = PlayerController->GetCameraDirector())
		{
			Director->OnPerspectiveChanged.AddUniqueDynamic(this, &ADBRiderCharacter::HandlePerspectiveChanged);
			HandlePerspectiveChanged(Director->GetPerspective());
		}
	}
}

void ADBRiderCharacter::HandlePerspectiveChanged(EDBPerspective Perspective)
{
	const ADBRiderPlayerController* PlayerController = Cast<ADBRiderPlayerController>(GetController());
	const bool bHideBody = Perspective == EDBPerspective::FirstPerson
		&& ShouldHideBodyInFirstPerson()
		&& (!PlayerController || !PlayerController->GetCameraDirector() || PlayerController->GetCameraDirector()->ShouldHideRiderBody());

	if (USkeletalMeshComponent* Mesh = GetMesh())
	{
		// OwnerNoSee keeps self-shadowing while hiding the body from our own view.
		Mesh->SetOwnerNoSee(bHideBody);
	}

	if (USkeletalMeshComponent* FirstPersonMesh = GetFirstPersonMesh())
	{
		FirstPersonMesh->SetVisibility(Perspective == EDBPerspective::FirstPerson, /*bPropagateToChildren=*/true);
	}
}

void ADBRiderCharacter::SetLocomotionContext(FGameplayTag NewContext)
{
	if (LocomotionContext == NewContext)
	{
		return;
	}

	const FGameplayTag PreviousContext = LocomotionContext;
	LocomotionContext = NewContext;
	ApplyLocomotionInputContexts();

	// Camera framing follows locomotion (OnFoot/Mounted/Flying mode pairs).
	if (ADBRiderPlayerController* PlayerController = Cast<ADBRiderPlayerController>(GetController()))
	{
		if (UDBCameraDirectorComponent* Director = PlayerController->GetCameraDirector())
		{
			Director->SetLocomotionContext(NewContext);
		}
	}

	OnLocomotionContextChanged.Broadcast(this, PreviousContext, NewContext);
}

void ADBRiderCharacter::ApplyLocomotionInputContexts()
{
	UDBInputConfig* Config = ResolveInputConfig();
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!Config || !PlayerController || !PlayerController->IsLocalController())
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!Subsystem)
	{
		return;
	}

	// Remove every locomotion context first. This keeps only one control scheme
	// active at a time and prevents mounted/flying actions from leaking into
	// on-foot play.
	for (const TObjectPtr<UInputMappingContext>& Context : Config->OnFootMappingContexts)
	{
		if (Context)
		{
			Subsystem->RemoveMappingContext(Context);
		}
	}
	for (const TObjectPtr<UInputMappingContext>& Context : Config->MountedMappingContexts)
	{
		if (Context)
		{
			Subsystem->RemoveMappingContext(Context);
		}
	}
	for (const TObjectPtr<UInputMappingContext>& Context : Config->FlyingMappingContexts)
	{
		if (Context)
		{
			Subsystem->RemoveMappingContext(Context);
		}
	}

	const TArray<TObjectPtr<UInputMappingContext>>* ActiveContexts = &Config->OnFootMappingContexts;
	if (LocomotionContext == DBGameplayTags::Locomotion_Mounted)
	{
		ActiveContexts = &Config->MountedMappingContexts;
	}
	else if (LocomotionContext == DBGameplayTags::Locomotion_Flying)
	{
		ActiveContexts = &Config->FlyingMappingContexts;
	}

	for (const TObjectPtr<UInputMappingContext>& Context : *ActiveContexts)
	{
		if (Context)
		{
			Subsystem->AddMappingContext(Context, 0);
		}
	}
}

void ADBRiderCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetController() && !MoveInput.IsNearlyZero())
	{
		// Camera-relative movement: forward/right from the control yaw.
		const FRotator ControlRotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Forward, MoveInput.Y);
		AddMovementInput(Right, MoveInput.X);
	}
}

void ADBRiderCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UDBInputConfig* Config = ResolveInputConfig();
	if (!Config)
	{
		UE_LOG(LogDragonbound, Error, TEXT("Rider '%s': no UDBInputConfig found — set it on the character or game mode (Docs/EDITOR_SETUP.md)."), *GetName());
		return;
	}

	// Apply the mapping set for the current locomotion context.\n\tApplyLocomotionInputContexts();\n\n	// Bind actions by identity; the asset owns keys/modifiers/remapping.
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (Config->MoveAction)
		{
			EnhancedInput->BindAction(Config->MoveAction, ETriggerEvent::Triggered, this, &ADBRiderCharacter::OnMoveTriggered);
			EnhancedInput->BindAction(Config->MoveAction, ETriggerEvent::Completed, this, &ADBRiderCharacter::OnMoveCompleted);
		}
		if (Config->LookAction)
		{
			EnhancedInput->BindAction(Config->LookAction, ETriggerEvent::Triggered, this, &ADBRiderCharacter::OnLookTriggered);
		}
		if (Config->JumpAction)
		{
			EnhancedInput->BindAction(Config->JumpAction, ETriggerEvent::Started, this, &ADBRiderCharacter::OnJumpStarted);
			EnhancedInput->BindAction(Config->JumpAction, ETriggerEvent::Completed, this, &ADBRiderCharacter::OnJumpCompleted);
		}
		if (Config->SprintAction)
		{
			EnhancedInput->BindAction(Config->SprintAction, ETriggerEvent::Started, this, &ADBRiderCharacter::OnSprintStarted);
			EnhancedInput->BindAction(Config->SprintAction, ETriggerEvent::Completed, this, &ADBRiderCharacter::OnSprintCompleted);
		}
		if (Config->TogglePerspectiveAction)
		{
			EnhancedInput->BindAction(Config->TogglePerspectiveAction, ETriggerEvent::Completed, this, &ADBRiderCharacter::OnTogglePerspective);
		}
		if (Config->InteractAction)
		{
			EnhancedInput->BindAction(Config->InteractAction, ETriggerEvent::Started, this, &ADBRiderCharacter::OnInteractStarted);
		}
	}
	else
	{
		UE_LOG(LogDragonbound, Error, TEXT("Rider '%s': input component is not Enhanced Input — bindings skipped."), *GetName());
	}
}

void ADBRiderCharacter::OnMoveTriggered(const FInputActionValue& Value)
{
	MoveInput = Value.Get<FVector2D>();
}

void ADBRiderCharacter::OnMoveCompleted(const FInputActionValue& Value)
{
	MoveInput = FVector2D::ZeroVector;
}

void ADBRiderCharacter::OnLookTriggered(const FInputActionValue& Value)
{
	const FVector2D Look = Value.Get<FVector2D>();
	AddControllerYawInput(Look.X * LookSensitivity);
	AddControllerPitchInput(Look.Y * LookSensitivity);

	// Clamp pitch to the active camera mode's limits so both perspectives
	// always render from a valid orientation.
	if (ADBRiderPlayerController* PlayerController = Cast<ADBRiderPlayerController>(GetController()))
	{
		if (UDBCameraDirectorComponent* Director = PlayerController->GetCameraDirector())
		{
			float MinPitch = -89.f;
			float MaxPitch = 89.f;
			Director->GetPitchLimits(MinPitch, MaxPitch);

			FRotator ControlRotation = PlayerController->GetControlRotation();
			ControlRotation.Pitch = FMath::ClampAngle(ControlRotation.Pitch, MinPitch, MaxPitch);
			PlayerController->SetControlRotation(ControlRotation);
		}
	}
}

void ADBRiderCharacter::OnJumpStarted(const FInputActionValue& Value)
{
	Jump();
}

void ADBRiderCharacter::OnJumpCompleted(const FInputActionValue& Value)
{
	StopJumping();
}

void ADBRiderCharacter::OnSprintStarted(const FInputActionValue& Value)
{
	if (UDBRiderMovementComponent* Movement = GetRiderMovementComponent())
	{
		Movement->SetSprinting(true);
	}
}

void ADBRiderCharacter::OnSprintCompleted(const FInputActionValue& Value)
{
	if (UDBRiderMovementComponent* Movement = GetRiderMovementComponent())
	{
		Movement->SetSprinting(false);
	}
}

void ADBRiderCharacter::OnTogglePerspective(const FInputActionValue& Value)
{
	if (ADBRiderPlayerController* PlayerController = Cast<ADBRiderPlayerController>(GetController()))
	{
		if (UDBCameraDirectorComponent* Director = PlayerController->GetCameraDirector())
		{
			Director->TogglePerspective();
		}
	}
}

void ADBRiderCharacter::OnInteractStarted(const FInputActionValue& Value)
{
	ADBRiderPlayerController* PlayerController = Cast<ADBRiderPlayerController>(GetController());
	if (!PlayerController)
	{
		return;
	}

	UDBInteractionComponent* Interaction = PlayerController->GetInteractionComponent();
	AActor* Target = Interaction ? Interaction->GetCurrentInteractionTarget() : nullptr;
	ADBDragonCharacter* Dragon = Cast<ADBDragonCharacter>(Target);
	if (!Dragon)
	{
		return;
	}

	if (ADBRiderPlayerController* RiderController = Cast<ADBRiderPlayerController>(PlayerController))
	{
		if (UDBControlRouterComponent* Router = RiderController->GetControlRouter())
		{
			Router->SubmitCommand(EDBControlCommand::Call);
		}
	}

	if (UDBDragonInteractionComponent* DragonInteraction = Dragon->GetInteractionComponent())
	{
		DragonInteraction->PerformInteraction(EDBDragonInteraction::Call, this);
	}
}
