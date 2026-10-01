#include "Characters/DBDragonEmotionComponent.h"

UDBDragonEmotionComponent::UDBDragonEmotionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDBDragonEmotionComponent::SetMood(EDBDragonMood NewMood, float NewIntensity)
{
	const EDBDragonMood PreviousMood = Mood;
	Mood = NewMood;
	Intensity = FMath::Clamp(NewIntensity, 0.f, 1.f);

	if (PreviousMood != Mood)
	{
		OnMoodChanged.Broadcast(PreviousMood, Mood);
	}
}

void UDBDragonEmotionComponent::ModifyIntensity(float Delta)
{
	Intensity = FMath::Clamp(Intensity + Delta, 0.f, 1.f);
}

void UDBDragonEmotionComponent::ApplyImpulse(float Delta)
{
	ModifyIntensity(Delta);
}