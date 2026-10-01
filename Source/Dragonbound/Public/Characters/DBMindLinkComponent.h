// Dragonbound — M2 mind-link channel.
//
// M2 supports emotional/sensory/image payloads only. Words and full dialogue
// remain locked to later bond stages.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DBMindLinkComponent.generated.h"

UENUM(BlueprintType)
enum class EDBMindLinkPayloadType : uint8
{
	Emotion,
	Sensation,
	Image
};

USTRUCT(BlueprintType)
struct DRAGONBOUND_API FDBMindLinkPayload
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDBMindLinkPayloadType Type = EDBMindLinkPayloadType::Emotion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Cue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Intensity = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 0.75f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDBMindLinkPayloadSignature, const FDBMindLinkPayload&, Payload);

UCLASS(ClassGroup=(Dragonbound), meta=(BlueprintSpawnableComponent))
class DRAGONBOUND_API UDBMindLinkComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDBMindLinkComponent();

	UFUNCTION(BlueprintCallable, Category="Dragon|Mind Link")
	void SendPayload(const FDBMindLinkPayload& Payload);

	UFUNCTION(BlueprintPure, Category="Dragon|Mind Link")
	bool CanSendFeelingPayloads() const { return true; }

	UPROPERTY(BlueprintAssignable, Category="Dragon|Mind Link")
	FDBMindLinkPayloadSignature OnPayloadReceived;
};