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

ADBRiderCharacter::ADBRiderCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UDBRiderMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	// The character never rotates from camera input; the camera is free
	// while the body follows movement direction.
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
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

	// Apply mapping contexts (authored per input scheme in the data asset).
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (PlayerController->IsLocalController())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				for (const TObjectPtr<UInputMappingContext>& MappingContext : Config->OnFootMappingContexts)
				{
					if (MappingContext)
					{
						Subsystem->AddMappingContext(MappingContext, 0);
					}
				}
			}
		}
	}

	// Bind actions by identity; the asset owns keys/modifiers/remapping.
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
		// InteractAction: consumed by the controller's interaction component
		// (UDBInteractionComponent); bound there once a gameplay use lands (M2 dragon interaction).
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
