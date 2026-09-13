// Dragonbound — base HUD.
//
// M1 responsibility: present the touch controls widget when the touch layer
// is active (phone-first; ADR-0003). The widget routes touch into the same
// Enhanced Input actions the character binds, so the HUD never talks to
// gameplay directly.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DBHUDBase.generated.h"

class UDBTouchControlsWidget;

UCLASS()
class DRAGONBOUND_API ADBHUDBase : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(Transient)
	TObjectPtr<UDBTouchControlsWidget> TouchControlsWidget;
};
