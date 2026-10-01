#include "Characters/DBMindLinkReceiverComponent.h"

UDBMindLinkReceiverComponent::UDBMindLinkReceiverComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDBMindLinkReceiverComponent::ReceivePayload(const FDBMindLinkPayload& Payload)
{
	LatestPayload = Payload;
	OnMindLinkReceived.Broadcast(Payload);
}