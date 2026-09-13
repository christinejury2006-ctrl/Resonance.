// Dragonbound — Rider game mode.
//
// M1 defaults: Rider pawn + controller, and team-wide fallback references
// for input and appearance (single-authoring point until per-player
// customization UI lands).

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DBRiderGameMode.generated.h"

class UDBInputConfig;
class UDBRiderAppearanceDefinition;
class UDBTouchConfig;

UCLASS()
class DRAGONBOUND_API ADBRiderGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADBRiderGameMode();

	/** Fallback input config when the character class doesn't provide one. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UDBInputConfig> DefaultInputConfig;

	/** Fallback Rider appearance (male/female definitions are authored in-editor). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Appearance")
	TObjectPtr<UDBRiderAppearanceDefinition> DefaultAppearanceDefinition;

	/** Touch controls config (phone-first input; KBM/gamepad secondary — ADR-0003). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Touch")
	TObjectPtr<UDBTouchConfig> DefaultTouchConfig;
};
