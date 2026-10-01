#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/DBInteractable.h"
#include "DBDragonCharacter.generated.h"
class UDBBondComponent; class UDBDragonEmotionComponent; class UDBMindLinkComponent; class UDBDragonInteractionComponent; class UDBDragonVisualComponent;
UCLASS() class DRAGONBOUND_API ADBDragonCharacter : public ACharacter, public IDBInteractable
{
 GENERATED_BODY()
public:
 ADBDragonCharacter(const FObjectInitializer& ObjectInitializer);
 UFUNCTION(BlueprintPure, Category="Dragon") UDBDragonEmotionComponent* GetEmotionComponent() const { return EmotionComponent; }
 UFUNCTION(BlueprintPure, Category="Dragon") UDBBondComponent* GetBondComponent() const { return BondComponent; }
 UFUNCTION(BlueprintPure, Category="Dragon") UDBMindLinkComponent* GetMindLinkComponent() const { return MindLinkComponent; }
 UFUNCTION(BlueprintPure, Category="Dragon") UDBDragonInteractionComponent* GetInteractionComponent() const { return InteractionComponent; }
 UFUNCTION(BlueprintPure, Category="Dragon") UDBDragonVisualComponent* GetVisualComponent() const { return VisualComponent; }
 virtual FText GetInteractionPrompt_Implementation() const override;
protected:
 virtual void BeginPlay() override;
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dragon|Components") TObjectPtr<UDBDragonEmotionComponent> EmotionComponent;
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dragon|Components") TObjectPtr<UDBBondComponent> BondComponent;
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dragon|Components") TObjectPtr<UDBMindLinkComponent> MindLinkComponent;
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dragon|Components") TObjectPtr<UDBDragonInteractionComponent> InteractionComponent;
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Dragon|Components") TObjectPtr<UDBDragonVisualComponent> VisualComponent;
};