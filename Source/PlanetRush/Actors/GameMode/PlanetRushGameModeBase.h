// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Image.h"
#include "LegacyCameraShake.h"
#include "Components/AudioComponent.h"
#include "Components/TimelineComponent.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlanetRushGameModeBase.generated.h"

class ABlock;
class AMoon;
class APlanetRushEnemy;
class APlanetRushCharacter;
/**
 *
 */
UCLASS()
class PLANETRUSH_API APlanetRushGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	APlanetRushGameModeBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void RegistBlock(ABlock* Block);
	void StartMeteorAttack(int32 Idx);

	void RegistMoon(AMoon* Moon);
	void UnRegistMoon(AMoon* Moon);
	void CheckMoon();

	void ToMainCamera();

public:
	void ZoomIn();
	void ShowGameOver();
	void ShowVictory();

protected:
	UFUNCTION(BlueprintCallable)
	void OpenTitleMap();

private:
	void EnableInput();
	void DisableInput();

public:
	APlanetRushEnemy* PlanetRushEnemy;
	APlanetRushCharacter* PlanetRushCharacter;
	ACameraActor* MainCamera;

	int32 CurrentMoonCount;
	bool bIsProcessingCutScene = true;
	bool bIsArriveEnemy = false;
	bool bIsProcessingGameOver = false;
	bool bIsProcessingVictory = false;

	UPROPERTY(EditAnywhere)
	FVector TargetLocationForCutScene = FVector(1000.0f, 0.0f, 1200.0f);

	UFUNCTION()
	void PlayRotateSound(float InTime);
	UFUNCTION()
	void StopRotateSound();

	FVector MouseWorldVector;

protected:
	UFUNCTION()
	void RotateForCutScene(float Value);
	UFUNCTION()
	void MoveEnemyForCutScene(float Value);
	UFUNCTION()
	void CameraShakeForCutScene();
	UFUNCTION()
	void StartCameraBlendForCutScene();
	UFUNCTION()
	void EndCutScene();

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ZoomWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> VictoryWidgetClass;

	UPROPERTY(EditAnywhere)
	float EnemyRotateForCutSceneTime = 5.0f;
	UPROPERTY(EditAnywhere)
	float CameraShakeTime = 2.0f;
	UPROPERTY(EditAnywhere)
	float CameraBlendTimeForCutScene = 2.0f;

	UPROPERTY(EditAnywhere)
	UAudioComponent* AudioComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTimelineComponent* TimelineComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* CurveFloat;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* BGM;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* GameVictorySound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* GameOverSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* RotateSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* ExplosionSound;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ULegacyCameraShake> CameraShakeClass;

private:
	TMap<int32, ABlock*> Blocks;
	int32 TotalBlockCount = 4;

	TArray<AMoon*> Moons;

	UPROPERTY(EditAnywhere)
	UUserWidget* ZoomWidgetUI;
	UPROPERTY(EditAnywhere)
	UUserWidget* GameOverWidgetUI;
	UPROPERTY(EditAnywhere)
	UUserWidget* VictoryWidgetUI;

	UImage* Image;
};
