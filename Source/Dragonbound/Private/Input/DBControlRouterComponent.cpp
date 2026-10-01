#include "Input/DBControlRouterComponent.h"

UDBControlRouterComponent::UDBControlRouterComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDBControlRouterComponent::SubmitCommand(EDBControlCommand Command)
{
	LastCommand = Command;
	OnCommand.Broadcast(Command);
}
