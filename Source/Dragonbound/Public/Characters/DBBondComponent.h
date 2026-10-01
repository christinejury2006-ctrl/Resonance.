// Dragonbound — M2 bond state.
//
// Bond is relationship state, not XP and not a GAS attribute. Persistent
// relationship events are recorded here so the dragon can remember how the
// Rider treated it.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "DBBondComponent.generated.h"

UENUM(BlueprintType)
enum class EDBBondStage : uint8
{
	Awakening,
	Feeling,
	Words,
	Dialogue,
	Unison
};

USTRUCT(BlueprintType)
struct DRAGONBOUND_API FDBondEvent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ContextTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Valence = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMemoryFlagged = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EventId;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDBondStageChangedSignature, EDBBondStage, PreviousStage, EDBBondStage, NewStage, float, BondDepth);

UCLASS(ClassGroup=(Dragonbound), meta=(BlueprintSpawnableComponent))
class DRAGONBOUND_API UDBBondComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDBBondComponent();

	UFUNCTION(BlueprintPure, Category="Dragon|Bond")
	EDBBondStage GetBondStage() const = delete;

	UFUNCTION(BlueprintPure, Category="Dragon|Bond")
	EDBBondStage GetBondStageInternal() const;

	UFUNCTION(BlueprintPure, Category="Dragon|Bond")
	EDBBondStage GetBondStageValue() const { return BondStage; }

	UFUNCTION(BlueprintPure, Category="Dragon|Bond")
	float GetBondDepth() const { return BondDepth; }

	UFUNCTION(BlueprintPure, Category="Dragon|Bond")
	float GetTrust() const { return Trust; }

	UFUNCTION(BlueprintCallable, Category="Dragon|Bond")
	void AdvanceStage(EDBBondStage NewStage);

	UFUNCTION(BlueprintCallable, Category="Dragon|Bond")
	void AddBondDepth(float Amount);

	UFUNCTION(BlueprintCallable, Category="Dragon|Bond")
	void AddTrust(float Amount);

	UFUNCTION(BlueprintCallable, Category="Dragon|Bond")
	void RecordEvent(const FDBondEvent& Event);

	UFUNCTION(BlueprintPure, Category="Dragon|Bond")
	const TArray<FDBondEvent>& GetMemory() const { return Memory; }

	UPROPERTY(BlueprintAssignable, Category="Dragon|Bond")
	FDBondStageChangedSignature OnBondStageChanged;

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Dragon|Bond")
	EDBBondStage BondStage = EDBBondStage::Awakening;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Dragon|Bond", meta=(ClampMin="0.0", ClampMax="100.0"))
	float BondDepth = 0.f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Dragon|Bond", meta=(ClampMin="0.0", ClampMax="100.0"))
	float Trust = 0.f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Dragon|Bond")
	TArray<FDBondEvent> Memory;
};