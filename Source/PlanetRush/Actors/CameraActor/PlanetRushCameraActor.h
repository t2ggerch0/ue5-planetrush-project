// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/PlanetRushCharacter.h"

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "PlanetRushCameraActor.generated.h"

/**
 * 
 */
UCLASS()
class PLANETRUSH_API APlanetRushCameraActor : public ACameraActor
{
	GENERATED_BODY()
	
public:
	APlanetRushCameraActor();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	ACharacter *PlayerCharacter;
};
