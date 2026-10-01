// Dragonbound — M2 dragon emotional state.
//
// Emotion is deliberately data-oriented and independent from AI. AI reads this
// component; gameplay events write to it. Animation/audio presentation can
// subscribe without owning the underlying state.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DBDragonEmotionComponent.generated.h"

UENUM(BlueprintType)
enum class EDBDragonMood : uint8
{
	Neutral,
	Curious,
	Comforted,
	Excited,
	Fearful,
	Distressed,
	Protective
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDBDragonMoodChangedSignature, EDBDragonMood, PreviousMood, EDBDragonMood, NewMood);

UCLASS(ClassGroup=(Dragonbound), meta=(BlueprintSpawnableComponent))
class DRAGONBOUND_API UDBDragonEmotionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDBDragonEmotionComponent();

	UFUNCTION(BlueprintPure, Category="Dragon|Emotion")
	EDBDragonMood GetMood() const { return Mood; }

	UFUNCTION(BlueprintPure, Category="Dragon|Emotion")
	float GetIntensity() const { return Intensity; }

	UFUNCTION(BlueprintCallable, Category="Dragon|Emotion")
	void SetMood(EDBDragonMood NewMood, float NewIntensity = 1.f);

	UFUNCTION(BlueprintCallable, Category="Dragon|Emotion")
	void ModifyIntensity(float Delta);

	/** Apply a bounded emotional impulse without changing the current mood. */
	UFUNCTION(BlueprintCallable, Category="Dragon|Emotion")
	void ApplyImpulse(float Delta);

	UPROPERTY(BlueprintAssignable, Category="Dragon|Emotion")
	FDBDragonMoodChangedSignature OnMoodChanged;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Dragon|Emotion", meta=(ClampMin="0.0", ClampMax="1.0"))
	float Intensity = 0.25f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Dragon|Emotion")
	EDBDragonMood Mood = EDBDragonMood::Neutral;
};