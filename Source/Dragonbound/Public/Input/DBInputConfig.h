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
	/** Mapping contexts applied while the Rider is on foot (M1). Mounted/Flying contexts are added with those systems. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TArray<TObjectPtr<UInputMappingContext>> OnFootMappingContexts;

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
