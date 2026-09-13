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
class UDBTouchControlLayer;
class UDBCameraDirectorComponent;
class UCameraComponent;

UCLASS()
class DRAGONBOUND_API ADBRiderPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ADBRiderPlayerController();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "Camera")
	UDBCameraDirectorComponent* GetCameraDirector() const { return CameraDirector; }

	UFUNCTION(BlueprintPure, Category = "Interaction")
	UDBInteractionComponent* GetInteractionComponent() const { return InteractionComponent; }

	/** Touch-first input layer (phone primary; KBM/gamepad secondary — ADR-0003). */
	UFUNCTION(BlueprintPure, Category = "Touch")
	UDBTouchControlLayer* GetTouchControlLayer() const { return TouchControlLayer; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UDBCameraDirectorComponent> CameraDirector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<UDBInteractionComponent> InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Touch")
	TObjectPtr<UDBTouchControlLayer> TouchControlLayer;

	/** The camera the director drives (view output comes from the director's POV via ADBPlayerCameraManager). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComponent;
};
