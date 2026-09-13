// Dragonbound — Rider appearance definition.
//
// Male/female Rider selection is wired at the asset level: each option is a
// data asset pointing at the body mesh, animation class, and collision
// sizing. M1 ships with placeholder avatars (engine mannequins); MetaHuman
// bodies replace the mesh references later without code changes
// (Docs/VISUAL_STYLE.md §2.2).

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DBRiderAppearanceDefinition.generated.h"

class UAnimInstance;
class USkeletalMesh;

UENUM(BlueprintType)
enum class ERiderSex : uint8
{
	Male,
	Female
};

UCLASS(BlueprintType, Blueprintable)
class DRAGONBOUND_API UDBRiderAppearanceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rider")
	ERiderSex Sex = ERiderSex::Male;

	/** Placeholder in M1 (engine mannequin); MetaHuman body later. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rider")
	TSoftObjectPtr<USkeletalMesh> BodyMesh;

	/** Placeholder animation blueprint in M1. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rider")
	TSoftClassPtr<UAnimInstance> AnimInstanceClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rider", meta = (ClampMin = "1.0"))
	float CapsuleRadius = 34.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rider", meta = (ClampMin = "1.0"))
	float CapsuleHalfHeight = 88.f;
};
