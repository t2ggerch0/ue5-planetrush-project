// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/GameMode/PlanetRushGameModeBase.h"

#include "Enemy/PlanetRushEnemy.h"
#include "Block/Block.h"
#include "Character/PlanetRushCharacter.h"
#include "Controller/PlanetRushPlayerController.h"

APlanetRushGameModeBase::APlanetRushGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;

	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->bAutoActivate = false;
}

void APlanetRushGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	DisableInput();

	// FInd PlanetRushEnemy
	TArray<AActor*> PlanetRushEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlanetRushEnemy::StaticClass()
		, PlanetRushEnemies);
	if (PlanetRushEnemies.Num() > 0)
	{
		PlanetRushEnemy = Cast<APlanetRushEnemy>(PlanetRushEnemies[0]);
	}

	// Get Character by controller
	APlanetRushPlayerController* PlayerController = Cast<APlanetRushPlayerController>
		(GetWorld()->GetFirstPlayerController());
	if (PlayerController)
	{
		PlanetRushCharacter = Cast<APlanetRushCharacter>(PlayerController->GetCharacter());
	}


	// FInd MainCamera
	TArray<AActor*> MainCameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld()
		, ACameraActor::StaticClass(), MainCameras);
	MainCamera = Cast<ACameraActor>(MainCameras[0]);

	// Start CutScene
	APlanetRushGameModeBase* GameMode = Cast<APlanetRushGameModeBase>
		(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		PlayRotateSound(EnemyRotateForCutSceneTime);
	}

	FOnTimelineFloat UpdateFunctionRotate;
	FOnTimelineFloat UpdateFunctionMove;
	FOnTimelineEvent TimelineFinished;
	UpdateFunctionRotate.BindUFunction(this, FName("RotateForCutScene"));
	UpdateFunctionMove.BindUFunction(this, FName("MoveEnemyForCutScene"));
	TimelineFinished.BindUFunction(this, FName("CameraShakeForCutScene"));

	TimelineComponent->AddInterpFloat(CurveFloat, UpdateFunctionRotate);
	TimelineComponent->AddInterpFloat(CurveFloat, UpdateFunctionMove);
	TimelineComponent->SetTimelineFinishedFunc(TimelineFinished);
	TimelineComponent->SetTimelineLength(EnemyRotateForCutSceneTime);
	TimelineComponent->SetLooping(false);
	TimelineComponent->PlayFromStart();
}

void APlanetRushGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsArriveEnemy)
	{
		if (FVector::Distance(PlanetRushEnemy->GetActorLocation(), TargetLocationForCutScene) < 100.0f)
		{
			PlanetRushEnemy->SetActorLocation(TargetLocationForCutScene);
			TimelineComponent->Stop();
			StopRotateSound();
			CameraShakeForCutScene();
			bIsArriveEnemy = true;
		}
	}

	if (!bIsProcessingCutScene)
	{
		// Get Mouse Position
		APlanetRushPlayerController* PlayerController = Cast<APlanetRushPlayerController>
			(GetWorld()->GetFirstPlayerController());
		if (PlayerController)
		{
			FVector2D MousePosition;
			PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);

			int32 X, Y;
			PlayerController->GetViewportSize(X, Y);

			MousePosition -= FVector2D(X / 2, Y / 2);
			Image->SetRenderTranslation(MousePosition);

			FVector WorldLocation, WorldDirection;
			FVector OutHitLocation;
			if (PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
			{
				FVector Start = WorldLocation;
				FVector End = Start + (WorldDirection * 10000.0f); 

				FHitResult HitResult;
				if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility))
				{
					OutHitLocation = HitResult.Location;
				}
			}


			MouseWorldVector = FVector(OutHitLocation.X, OutHitLocation.Y
				, PlanetRushCharacter->GetActorLocation().Z);
			PlanetRushCharacter->SetMuzzleLookAt(MouseWorldVector);
		}
	}
};

#pragma region CutScene
void APlanetRushGameModeBase::RotateForCutScene(float Value)
{
	PlanetRushEnemy->RotateForCutScene(Value);
}

void APlanetRushGameModeBase::MoveEnemyForCutScene(float Value)
{
	FVector Location = PlanetRushEnemy->GetActorLocation();
	Location.X += Value * 50.0f;
	PlanetRushEnemy->SetActorLocation(Location);
}

void APlanetRushGameModeBase::CameraShakeForCutScene()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ExplosionSound);

	// Shake Camera
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->ClientStartCameraShake(CameraShakeClass, 1.0f);

	// Move Blocks
	for (auto& Element : Blocks)
	{
		Blocks[Element.Key]->MoveBlock();
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this
		, &APlanetRushGameModeBase::StartCameraBlendForCutScene, CameraShakeTime, false);
}

void APlanetRushGameModeBase::StartCameraBlendForCutScene()
{
	ToMainCamera();
	UGameplayStatics::PlaySound2D(GetWorld(), BGM);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle
		, this, &APlanetRushGameModeBase::EndCutScene, CameraBlendTimeForCutScene, false);
}

void APlanetRushGameModeBase::ToMainCamera()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->SetViewTargetWithBlend(MainCamera, CameraBlendTimeForCutScene, VTBlend_Cubic);
}

void APlanetRushGameModeBase::EndCutScene()
{
	ZoomIn();
	EnableInput();
	bIsProcessingCutScene = false;
}

void APlanetRushGameModeBase::ZoomIn()
{
	ZoomWidgetUI = CreateWidget<UUserWidget>(GetWorld(), ZoomWidgetClass);
	ZoomWidgetUI->AddToViewport();
	Image = Cast<UImage>(ZoomWidgetUI->GetWidgetFromName(TEXT("Image")));
}
#pragma endregion

void APlanetRushGameModeBase::PlayRotateSound(float InTime)
{
	AudioComponent->SetSound(RotateSound);
	AudioComponent->Play();

	FTimerHandle SoundStopTimerHandle;
	if (GetWorld() && IsValid(this))
	{
		GetWorldTimerManager().SetTimer(SoundStopTimerHandle
			, this, &APlanetRushGameModeBase::StopRotateSound, InTime, false);
	}
}

void APlanetRushGameModeBase::StopRotateSound()
{
	if (!AudioComponent)
	{
		return;
	}

	if (AudioComponent->IsPlaying())
	{
		AudioComponent->Stop();
	}
}

void APlanetRushGameModeBase::EnableInput()
{
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
}

void APlanetRushGameModeBase::DisableInput()
{
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}

void APlanetRushGameModeBase::RegistBlock(ABlock* Block)
{
	Blocks.Add(Block->GetIdx(), Block);
}

void APlanetRushGameModeBase::StartMeteorAttack(int32 Count)
{
	TSet<int32> Set;
	do
	{
		int32 RandomInt = FMath::RandRange(0, TotalBlockCount);
		Set.Add(RandomInt);
	} while (Set.Num() < Count);

	for (auto& Element : Set)
	{
		if (Blocks.Contains(Element))
		{
			Blocks[Element]->CallMeteor();
		}
	}
}

void APlanetRushGameModeBase::RegistMoon(AMoon* Moon)
{
	Moons.Add(Moon);
}

void APlanetRushGameModeBase::UnRegistMoon(AMoon* Moon)
{
	Moons.Remove(Moon);
}

void APlanetRushGameModeBase::CheckMoon()
{
	if (Moons.Num() == 0)
	{
		ShowVictory();
	}
}

void APlanetRushGameModeBase::ShowVictory()
{
	if (bIsProcessingVictory || bIsProcessingGameOver)
	{
		return;
	}
	bIsProcessingVictory = true;

	UGameplayStatics::PlaySound2D(GetWorld(), GameVictorySound);

	VictoryWidgetUI = CreateWidget<UUserWidget>(GetWorld(), VictoryWidgetClass);
	VictoryWidgetUI->AddToViewport();

	DisableInput();
	PlanetRushCharacter->SetActorHiddenInGame(true);

	// 3초뒤 Title Map으로 OpenLevel
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer
	(TimerHandle, this, &APlanetRushGameModeBase::OpenTitleMap, 2.0f, false);
}

void APlanetRushGameModeBase::ShowGameOver()
{
	if (bIsProcessingGameOver || bIsProcessingVictory)
	{
		return;
	}
	bIsProcessingGameOver = true;

	UGameplayStatics::PlaySound2D(GetWorld(), GameOverSound);

	GameOverWidgetUI = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
	GameOverWidgetUI->AddToViewport();

	DisableInput();
	PlanetRushCharacter->SetActorHiddenInGame(true);

	// 3초뒤 Title Map으로 OpenLevel
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer
	(TimerHandle, this, &APlanetRushGameModeBase::OpenTitleMap, 2.0f, false);
}

void APlanetRushGameModeBase::OpenTitleMap()
{
	EnableInput();
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("TitleMap"));
}

