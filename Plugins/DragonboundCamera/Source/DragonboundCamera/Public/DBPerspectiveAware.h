// DragonboundCamera — perspective-aware actors.
//
// Implemented by view targets that react to perspective changes (the Rider
// hides its full body mesh in first person and shows a first-person mesh
// override when one exists — see ADBRiderCharacter).

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DBPerspectiveAware.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UDBPerspectiveAware : public UInterface
{
	GENERATED_BODY()
};

class DRAGONBOUNDCAMERA_API IDBPerspectiveAware
{
	GENERATED_BODY()

public:
	/** Whether the implementer should hide its full-body mesh while in first person. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Camera")
	bool ShouldHideBodyInFirstPerson() const;

	virtual bool ShouldHideBodyInFirstPerson_Implementation() const { return true; }
};
