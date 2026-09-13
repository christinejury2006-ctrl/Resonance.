// Dragonbound — player state base implementation.

#include "Game/DBPlayerStateBase.h"
#include "Save/DBSaveGame.h"

void UDBPlayerStateBase::WriteToSaveGame(UDBSaveGame* SaveGame) const
{
	// Intentionally empty in M1. Systems that add persistent player state
	// (bond, progression, mastery — M2/M3) extend this contract.
}

void UDBPlayerStateBase::ReadFromSaveGame(const UDBSaveGame* SaveGame)
{
	// Intentionally empty in M1; mirrors WriteToSaveGame.
}
