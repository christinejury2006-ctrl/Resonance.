// Dragonbound — save game object.
//
// Minimal, versioned save boundary (M1). The version constant exists from
// day one so that every future schema change is an explicit, migratable
// step (Docs/SYSTEMS.md §9). Payload fields land with their systems.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DBSaveGame.generated.h"

UCLASS(BlueprintType)
class DRAGONBOUND_API UDBSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/** Current save schema version. Bump with every breaking schema change and add a migration step. */
	static constexpr int32 CurrentSchemaVersion = 1;

	/** Version of the schema this save was written with. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
	int32 SchemaVersion = CurrentSchemaVersion;
};
