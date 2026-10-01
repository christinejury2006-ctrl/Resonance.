#include "Characters/DBMindLinkComponent.h"

UDBMindLinkComponent::UDBMindLinkComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDBMindLinkComponent::SendPayload(const FDBMindLinkPayload& Payload)
{
	if (Payload.Cue.IsNone() || Payload.Intensity <= 0.f)
	{
		return;
	}

	// M2 presentation is intentionally event-driven. The receiving Rider-side
	// presentation layer will subscribe once the interaction/mind-speech
	// channel is wired.
	OnPayloadReceived.Broadcast(Payload);
}