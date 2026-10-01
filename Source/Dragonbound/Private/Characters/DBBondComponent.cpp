#include "Characters/DBBondComponent.h"

UDBBondComponent::UDBBondComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDBBondComponent::AdvanceStage(EDBBondStage NewStage)
{
	if (static_cast<uint8>(NewStage) <= static_cast<uint8>(BondStage))
	{
		return;
	}

	const EDBBondStage PreviousStage = BondStage;
	BondStage = NewStage;
	OnBondStageChanged.Broadcast(PreviousStage, BondStage, BondDepth);
}

void UDBBondComponent::AddBondDepth(float Amount)
{
	BondDepth = FMath::Clamp(BondDepth + Amount, 0.f, 100.f);
}

void UDBBondComponent::AddTrust(float Amount)
{
	Trust = FMath::Clamp(Trust + Amount, 0.f, 100.f);
}

void UDBBondComponent::RecordEvent(const FDBondEvent& Event)
{
	Memory.Add(Event);

	// Significant positive/negative events can influence relationship state,
	// but stage progression itself remains story-driven.
	if (Event.bMemoryFlagged)
	{
		AddBondDepth(FMath::Clamp(Event.Valence, -5.f, 5.f));
		AddTrust(Event.Valence * 0.5f);
	}
}