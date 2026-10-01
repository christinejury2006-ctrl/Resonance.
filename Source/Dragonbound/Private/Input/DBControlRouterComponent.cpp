#include "Input/DBControlRouterComponent.h"
#include "DBNativeGameplayTags.h"

UDBControlRouterComponent::UDBControlRouterComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDBControlRouterComponent::SubmitCommand(EDBControlCommand Command)
{
	if (!CanExecuteCommand(Command))
	{
		return;
	}

	LastCommand = Command;
	OnCommand.Broadcast(Command);
}

void UDBControlRouterComponent::SetLocomotionContext(FGameplayTag NewContext)
{
	LocomotionContext = NewContext;
}

bool UDBControlRouterComponent::CanExecuteCommand(EDBControlCommand Command) const
{
	if (Command == EDBControlCommand::None)
	{
		return false;
	}

	if (Command == EDBControlCommand::Mount)
	{
		return LocomotionContext == DBGameplayTags::Locomotion_OnFoot;
	}

	if (Command == EDBControlCommand::Dismount)
	{
		return LocomotionContext == DBGameplayTags::Locomotion_Mounted
			|| LocomotionContext == DBGameplayTags::Locomotion_Flying;
	}

	if (Command == EDBControlCommand::PrimaryAbility || Command == EDBControlCommand::SecondaryAbility)
	{
		return LocomotionContext == DBGameplayTags::Locomotion_Mounted
			|| LocomotionContext == DBGameplayTags::Locomotion_Flying;
	}

	return LocomotionContext == DBGameplayTags::Locomotion_OnFoot;
}
