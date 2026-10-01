#include "Characters/DBDragonInteractionComponent.h"
#include "Characters/DBDragonEmotionComponent.h"
#include "Characters/DBBondComponent.h"
#include "Characters/DBMindLinkComponent.h"
#include "Characters/DBDragonCharacter.h"
#include "GameFramework/Character.h"

UDBDragonInteractionComponent::UDBDragonInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UDBDragonInteractionComponent::IsInteractionAvailable(EDBDragonInteraction Interaction) const
{
	return GetOwner() && static_cast<uint8>(Interaction) <= static_cast<uint8>(EDBDragonInteraction::Protect);
}

bool UDBDragonInteractionComponent::PerformInteraction(EDBDragonInteraction Interaction, AActor* Instigator)
{
	if (!IsInteractionAvailable(Interaction) || !Instigator)
	{
		return false;
	}

	ApplyInteraction(Interaction, Instigator);
	OnInteraction.Broadcast(Interaction, Instigator);
	return true;
}

void UDBDragonInteractionComponent::ApplyInteraction(EDBDragonInteraction Interaction, AActor* Instigator)
{
	ADBDragonCharacter* Dragon = Cast<ADBDragonCharacter>(GetOwner());
	if (!Dragon)
	{
		return;
	}

	UDBDragonEmotionComponent* Emotion = Dragon->GetEmotionComponent();
	UDBBondComponent* Bond = Dragon->GetBondComponent();
	UDBMindLinkComponent* Link = Dragon->GetMindLinkComponent();

	FDBondEvent Event;
	Event.EventId = FName(*UEnum::GetValueAsString(Interaction));

	switch (Interaction)
	{
	case EDBDragonInteraction::Call:
		if (Emotion) Emotion->SetMood(EDBDragonMood::Curious, 0.7f);
		Event.Valence = 1.f;
		Event.bMemoryFlagged = false;
		break;

	case EDBDragonInteraction::Feed:
		if (Emotion) Emotion->SetMood(EDBDragonMood::Comforted, 0.8f);
		Event.Valence = 4.f;
		Event.bMemoryFlagged = true;
		if (Link)
		{
			Link->SendPayload({EDBMindLinkPayloadType::Sensation, FName(TEXT("Warmth")), 0.65f, 1.0f});
		}
		break;

	case EDBDragonInteraction::Soothe:
		if (Emotion) Emotion->SetMood(EDBDragonMood::Comforted, 0.9f);
		Event.Valence = 5.f;
		Event.bMemoryFlagged = true;
		FDBMindLinkPayload Payload;
		Payload.Type = EDBMindLinkPayloadType::Emotion;
		Payload.Cue = FName(TEXT("Safety"));
		Payload.Intensity = 0.85f;
		Payload.Duration = 1.25f;
		if (Link)
		{
			Link->SendPayload(Payload);
		}
		if (ADBRiderCharacter* Rider = Cast<ADBRiderCharacter>(Instigator))
		{
			if (UDBMindLinkReceiverComponent* Receiver = Rider->GetMindLinkReceiver())
			{
				Receiver->ReceivePayload(Payload);
			}
		}
		break;

	case EDBDragonInteraction::Protect:
		if (Emotion) Emotion->SetMood(EDBDragonMood::Protective, 0.8f);
		Event.Valence = 6.f;
		Event.bMemoryFlagged = true;
		break;
	}

	if (Bond)
	{
		Bond->RecordEvent(Event);
		if (Event.bMemoryFlagged && Bond->GetBondDepth() >= 10.f)
		{
			Bond->AdvanceStage(EDBBondStage::Feeling);
		}
	}
}