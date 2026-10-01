#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "DBControlRouterComponent.generated.h"

UENUM(BlueprintType)
enum class EDBControlCommand : uint8
{
	None,
	Call,
	Feed,
	Soothe,
	Protect,
	Mount,
	Dismount,
	PrimaryAbility,
	SecondaryAbility
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDBControlCommandSignature, EDBControlCommand, Command);

/**
 * Central gameplay-command router for player controls.
 *
 * Keyboard/gamepad/mobile UI should eventually feed commands through this
 * component instead of calling dragon systems directly. The active
 * locomotion
 * mapping determines which physical input produces a command; gameplay systems
 * consume the command without caring about the device.
 */
UCLASS(ClassGroup=(Dragonbound), meta=(BlueprintSpawnableComponent))
class DRAGONBOUND_API UDBControlRouterComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDBControlRouterComponent();

	UFUNCTION(BlueprintCallable, Category="Controls")
	void SubmitCommand(EDBControlCommand Command);

	UFUNCTION(BlueprintCallable, Category="Controls")
	void SetLocomotionContext(FGameplayTag NewContext);

	UFUNCTION(BlueprintPure, Category="Controls")
	bool CanExecuteCommand(EDBControlCommand Command) const;

	UFUNCTION(BlueprintPure, Category="Controls")
	FGameplayTag GetLocomotionContext() const { return LocomotionContext; }

	UFUNCTION(BlueprintPure, Category="Controls")
	EDBControlCommand GetLastCommand() const { return LastCommand; }

	UPROPERTY(BlueprintAssignable, Category="Controls")
	FDBControlCommandSignature OnCommand;

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Controls", meta=(AllowPrivateAccess="true"))
	EDBControlCommand LastCommand = EDBControlCommand::None;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Controls", meta=(AllowPrivateAccess="true"))
	FGameplayTag LocomotionContext;
};
