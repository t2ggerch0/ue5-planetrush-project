// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameMode/PlanetRushGameModeBase.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/UserWidget.h"
#include "Subsystem/DataSubsystem.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlanetRushCharacter.generated.h"


UCLASS()
class PLANETRUSH_API APlanetRushCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlanetRushCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other,
		UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation,
		FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void Fire();
	void Replace();

	void SetMuzzleLookAt(FVector TargetLocation);

protected:
	void ShowGameOver();

protected:
	EProjectileType CurrentProjectileType = EProjectileType::Fire;

protected:
	UPROPERTY(EditAnywhere)
	USpringArmComponent* CameraSpringArmComponent;
	UPROPERTY(EditAnywhere)
	UCameraComponent* DefaultCamera;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Head;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Body;
	UPROPERTY(EditAnywhere)
	USpringArmComponent* GunSpringArmComponent;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Gun;
	UPROPERTY(EditAnywhere)
	USceneComponent* Muzzle;
	UPROPERTY(EditAnywhere)
	UArrowComponent* Arrow;
	UPROPERTY(EditAnywhere)
	UCameraComponent* ZoomCamera;

private:
	FTimerHandle FireTimerHandle;
};
