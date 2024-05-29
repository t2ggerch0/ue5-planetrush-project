// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MISC/MISC.h"

#include "Components/AudioComponent.h"
#include "Moon.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "AIController.h"
#include "Components/TimelineComponent.h"
#include "Components/SceneComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlanetRushEnemy.generated.h"

UCLASS()
class PLANETRUSH_API APlanetRushEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlanetRushEnemy();

protected:
	// Called when the game starts or when spawned

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void RotateForCutScene(float Value);

protected:
	UFUNCTION()
	void StartWait();
	UFUNCTION()
	void StartWaitInternal();
	UFUNCTION()
	void StartMeteorAttack();
	UFUNCTION()
	void OnEndMeteorAttack();
	UFUNCTION()
	void RotateInMeteorAttack(float Value);
	UFUNCTION()
	void StartMeteorAttackInternal();
	UFUNCTION()
	void StartMeteorAttackGameMode();

protected:
	UFUNCTION()
	void StartMoonAttack();
	UFUNCTION()
	void StartMoonAttackInternal();
	UFUNCTION()
	void OnEndMoonAttack();
	UFUNCTION()
	void RotateInMoonAttack(float Value);


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* EnemyRootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Earth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Cloud;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTimelineComponent* TimelineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector4 UVParam = FVector4(1.0f, 1.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere)
	TSubclassOf<AMoon> MoonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* CurveFloat;

	UPROPERTY(EditAnywhere)
	UMaterialInstanceDynamic* EarthMaterialInstanceDynamic;

	UPROPERTY(EditAnywhere)
	UMaterialInstanceDynamic* CloudMaterialInstanceDynamic;

private:
	TSubclassOf<AAIController> EnemyAIControllerBPClass;

	float MeteorAttackTime = 10.0f;
	float MoonAttackTime = 10.0f;

	int32 MeteorCountPerSecond = 2;
	
	int32 bIsProcessing = false;
	int32 PrevState;

	int32 RandomValue = 0;
};
