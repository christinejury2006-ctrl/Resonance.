// Dragonbound — input configuration data asset.
//
// Authored in-editor (see Docs/EDITOR_SETUP.md): one asset per input scheme.
// C++ binds these actions by identity; the mapping contexts and modifiers
// (mouse vs. gamepad, axis negation) live in the asset, so control remapping
// and context switching never require code changes.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DBInputConfig.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS(BlueprintType, Blueprintable)
class DRAGONBOUND_API UDBInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	/** Mapping contexts used while the Rider is on foot. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Contexts")
	TArray<TObjectPtr<UInputMappingContext>> OnFootMappingContexts;

	/** Mapping contexts used while mounted. Kept separate so the control layer can swap contexts without rebinding actions. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Contexts")
	TArray<TObjectPtr<UInputMappingContext>> MountedMappingContexts;

	/** Mapping contexts used while flying. Kept separate for the eventual 3D flight control scheme. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Contexts")
	TArray<TObjectPtr<UInputMappingContext>> FlyingMappingContexts;

	/** Axis2D — camera-relative movement. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	/** Axis2D — look (mouse delta / right stick). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	/** Digital — jump. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	/** Digital — sprint (held). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> SprintAction;

	/** Digital — toggle first/third person. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> TogglePerspectiveAction;

	/** Digital — interaction (dragon interaction and world interaction reuse this action). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> InteractAction;
};
