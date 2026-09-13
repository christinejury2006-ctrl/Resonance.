// Dragonbound — player state base.
//
// The persistence boundary for per-player state. M1 only establishes the
// save round-trip contract (schema-versioned UDBSaveGame); the full save
// system and its fields land with the first persistent systems (M2/M3).

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DBPlayerStateBase.generated.h"

class UDBSaveGame;

UCLASS()
class DRAGONBOUND_API UDBPlayerStateBase : public APlayerState
{
	GENERATED_BODY()

public:
	/** Write this player's persistent state into the save game. Filled out as systems land. */
	UFUNCTION(BlueprintCallable, Category = "Save")
	virtual void WriteToSaveGame(UDBSaveGame* SaveGame) const;

	/** Restore this player's persistent state from the save game. */
	UFUNCTION(BlueprintCallable, Category = "Save")
	virtual void ReadFromSaveGame(const UDBSaveGame* SaveGame);
};
