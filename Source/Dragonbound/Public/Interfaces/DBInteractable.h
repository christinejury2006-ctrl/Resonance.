// Dragonbound — interactable interface.
//
// Anything the interaction trace can target implements this (world objects
// in M1; the dragon companion in M2). The trace itself lives in
// UDBInteractionComponent; gameplay responses to interaction land with the
// systems that need them (no gameplay is wired to this in M1).

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DBInteractable.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UDBInteractable : public UInterface
{
	GENERATED_BODY()
};

class DRAGONBOUND_API IDBInteractable
{
	GENERATED_BODY()

public:
	/** Short prompt shown to the player (e.g. "Call to the dragon" — later). */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	FText GetInteractionPrompt() const;

	virtual FText GetInteractionPrompt_Implementation() const
	{
		return NSLOCTEXT("DBInteraction", "DefaultPrompt", "Interact");
	}
};
