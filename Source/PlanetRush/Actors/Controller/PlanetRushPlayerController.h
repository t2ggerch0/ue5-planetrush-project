// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Data/Input/BasicInputDataConfig.h"
#include "Actors/CameraManager/PlanetRushPlayerCameraManager.h"
#include "Character/PlanetRushCharacter.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlanetRushPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PLANETRUSH_API APlanetRushPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APlanetRushPlayerController();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

protected:
	void OnMove(const FInputActionValue& InputActionValue);
	void OnFire(const FInputActionValue& InputActionValue);
	void OnReplace(const FInputActionValue& InputActionValue);
	void OnDilation(const FInputActionValue& InputActionValue);
	void OnToTitle(const FInputActionValue& InputActionValue);
};
