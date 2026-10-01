// Dragonbound — M2 Rider-side mind-link receiver.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/DBMindLinkComponent.h"
#include "DBMindLinkReceiverComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDBMindLinkReceivedSignature, const FDBMindLinkPayload&, Payload);

UCLASS(ClassGroup=(Dragonbound), meta=(BlueprintSpawnableComponent))
class DRAGONBOUND_API UDBMindLinkReceiverComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDBMindLinkReceiverComponent();

	UFUNCTION(BlueprintCallable, Category="Dragon|Mind Link")
	void ReceivePayload(const FDBMindLinkPayload& Payload);

	UFUNCTION(BlueprintPure, Category="Dragon|Mind Link")
	FDBMindLinkPayload GetLatestPayload() const { return LatestPayload; }

	UPROPERTY(BlueprintAssignable, Category="Dragon|Mind Link")
	FDBMindLinkReceivedSignature OnMindLinkReceived;

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Dragon|Mind Link")
	FDBMindLinkPayload LatestPayload;
};