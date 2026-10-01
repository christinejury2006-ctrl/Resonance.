#include "Characters/DBDragonVisualComponent.h"

UDBDragonVisualComponent::UDBDragonVisualComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SilhouetteProfile = NSLOCTEXT("DragonVisual", "HeroSilhouette", "Lean quadruped; elongated neck and head; large swept membrane wings; long expressive tail; natural scale layering.");
}

void UDBDragonVisualComponent::SetGrowthStage(EDBDragonGrowthStage NewStage)
{
	GrowthStage = NewStage;
}

void UDBDragonVisualComponent::SetVisualState(EDBDragonVisualState NewState)
{
	VisualState = NewState;
}
