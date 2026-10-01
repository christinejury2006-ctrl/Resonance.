// Dragonbound — M2 dragon visual state.
//
// Owns presentation state that can be consumed by Blueprint/Animation/Material
// layers. Final skeletal meshes and animation assets remain editor-authored.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DBDragonVisualComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDBDragonVisualStateChangedSignature, EDBDragonVisualState, PreviousState, EDBDragonVisualState, NewState);

UENUM(BlueprintType)
enum class EDBDragonGrowthStage : uint8
{
	Hatchling,
	Juvenile,
	YoungAdult,
	Adult
};

UENUM(BlueprintType)
enum class EDBDragonVisualState : uint8
{
	Calm,
	Alert,
	Comforted,
	Distressed,
	Protective
};

UCLASS(ClassGroup=(Dragonbound), meta=(BlueprintSpawnableComponent))
class DRAGONBOUND_API UDBDragonVisualComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDBDragonVisualComponent();

	UFUNCTION(BlueprintPure, Category="Dragon|Visual")
	EDBDragonGrowthStage GetGrowthStage() const { return GrowthStage; }

	UFUNCTION(BlueprintCallable, Category="Dragon|Visual")
	void SetGrowthStage(EDBDragonGrowthStage NewStage);

	UFUNCTION(BlueprintPure, Category="Dragon|Visual")
	EDBDragonVisualState GetVisualState() const { return VisualState; }

	UFUNCTION(BlueprintCallable, Category="Dragon|Visual")
	void SetVisualState(EDBDragonVisualState NewState);

	UPROPERTY(BlueprintAssignable, Category="Dragon|Visual")
	FDBDragonVisualStateChangedSignature OnVisualStateChanged;

	/**
	 * Art-direction notes for the hero dragon: lean, long-tailed silhouette;
	 * large swept membrane wings; elongated neck/head; natural scale layering.
	 * These are intentionally metadata, not generated geometry.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dragon|Visual|Art Direction")
	FText SilhouetteProfile;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dragon|Visual")
	EDBDragonGrowthStage GrowthStage = EDBDragonGrowthStage::Juvenile;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Dragon|Visual")
	EDBDragonVisualState VisualState = EDBDragonVisualState::Calm;
};
