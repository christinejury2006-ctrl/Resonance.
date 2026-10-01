// Dragonbound — M2 juvenile dragon companion.
//
// The dragon is a co-protagonist, never a mount with a health bar. M2 starts
// with the companion pawn and relationship components; combat and flight land
// in their later milestones.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DBDragonCharacter.generated.h"

class UDBBondComponent;
class UDBDragonEmotionComponent;
class UDBMindLinkComponent;

UCLASS()
class DRAGONBOUND_API ADBDragonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ADBDragonCharacter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category="Dragon")
	UDBDragonEmotionComponent* GetEmotionComponent() const { return EmotionComponent; }

	UFUNCTION(BlueprintPure, Category="Dragon")
	UDBBondComponent* GetBondComponent() const { return BondComponent; }

	UFUNCTION(BlueprintPure, Category="Dragon")
	UDBMindLinkComponent* GetMindLinkComponent() const { return MindLinkComponent; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dragon|Components")
	TObjectPtr<UDBDragonEmotionComponent> EmotionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dragon|Components")
	TObjectPtr<UDBBondComponent> BondComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dragon|Components")
	TObjectPtr<UDBMindLinkComponent> MindLinkComponent;
};