// Dragonbound — Rider player controller.
//
// Owns the camera director (all camera work — ADR-0002), the camera
// component the director drives, and the interaction trace component
// (the foundation the dragon interaction builds on in M2).

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DBRiderPlayerController.generated.h"

class UDBInteractionComponent;
class UDBCameraDirectorComponent;
class UCameraComponent;

UCLASS()
class DRAGONBOUND_API ADBRiderPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ADBRiderPlayerController();

	UFUNCTION(BlueprintPure, Category = "Camera")
	UDBCameraDirectorComponent* GetCameraDirector() const { return CameraDirector; }

	UFUNCTION(BlueprintPure, Category = "Interaction")
	UDBInteractionComponent* GetInteractionComponent() const { return InteractionComponent; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UDBCameraDirectorComponent> CameraDirector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<UDBInteractionComponent> InteractionComponent;

	/** The camera the director drives (view output comes from the director's POV via ADBPlayerCameraManager). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComponent;
};
