#include "Actors/Enemy/PlanetRushEnemy.h"
#include "Character/PlanetRushCharacter.h"
#include "Projectile/Projectile.h"
#include "GameMode/PlanetRushGameModeBase.h"

APlanetRushEnemy::APlanetRushEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	EnemyRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("EnemyRootComponent"));
	Earth = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Earth"));
	Cloud = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cloud"));
	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));

	SetRootComponent(EnemyRootComponent);
	Earth->SetupAttachment(EnemyRootComponent);
	Cloud->SetupAttachment(EnemyRootComponent);

	AIControllerClass = EnemyAIControllerBPClass;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void APlanetRushEnemy::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UMaterialInterface* EarthMaterial = Earth->GetMaterial(0);
	EarthMaterialInstanceDynamic = Earth->CreateDynamicMaterialInstance(0, EarthMaterial);
	ensure(EarthMaterialInstanceDynamic);
	UMaterialInterface* CloudMaterial = Cloud->GetMaterial(0);
	CloudMaterialInstanceDynamic = Cloud->CreateDynamicMaterialInstance(0, CloudMaterial);
	ensure(CloudMaterialInstanceDynamic);

	EarthMaterialInstanceDynamic->SetVectorParameterValue(TEXT("UVParam"), UVParam);
	CloudMaterialInstanceDynamic->SetVectorParameterValue(TEXT("UVParam"), UVParam);

	FVector ForwardVector = GetActorForwardVector();
	EarthMaterialInstanceDynamic->SetVectorParameterValue(TEXT("ForwardVector"), ForwardVector);
}

void APlanetRushEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called to bind functionality to input
void APlanetRushEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlanetRushEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlanetRushEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlanetRushGameModeBase* GameMode = Cast<APlanetRushGameModeBase>
		(GetWorld()->GetAuthGameMode());
	if (!GameMode || GameMode->bIsProcessingCutScene)
	{
		return;
	}

	if (bIsProcessing)
	{
		return;
	}
	bIsProcessing = true;

	// 0부터 2까지의 랜덤 정수값 생성
	/*do
	{
		RandomValue = FMath::RandRange(0, 2);
	} while (RandomValue == PrevState);
	PrevState = RandomValue;*/

	if (RandomValue == 0)
	{
		// 메테오 공격
		StartMeteorAttack();
		RandomValue = 2;
	}
	else if (RandomValue == 1)
	{
		// 달 공격
		StartMoonAttack();
	}
	else
	{
		StartWait();
		RandomValue = 1;
	}
}

void APlanetRushEnemy::StartWait()
{
	int RandomWaitValue = FMath::RandRange(1, 3);
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this
		, &APlanetRushEnemy::StartWaitInternal, RandomValue, false);
}

void APlanetRushEnemy::StartWaitInternal()
{
	bIsProcessing = false;
}

#pragma region Meteor Attack
void APlanetRushEnemy::StartMeteorAttack()
{
	APlanetRushGameModeBase* GameMode = Cast<APlanetRushGameModeBase>
		(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->PlayRotateSound(MeteorAttackTime);
	}

	FOnTimelineFloat UpdateFunction;
	FOnTimelineEvent TimelineFinished;
	UpdateFunction.BindUFunction(this, FName("RotateInMeteorAttack"));
	TimelineFinished.BindUFunction(this, FName("OnEndMeteorAttack"));

	TimelineComponent->AddInterpFloat(CurveFloat, UpdateFunction);
	TimelineComponent->SetTimelineFinishedFunc(TimelineFinished);
	TimelineComponent->SetTimelineLength(MeteorAttackTime);
	TimelineComponent->SetLooping(false);
	TimelineComponent->PlayFromStart();

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this
		, &APlanetRushEnemy::StartMeteorAttackInternal, 1.0f, false);
}

void APlanetRushEnemy::OnEndMeteorAttack()
{
	StartWait();
}

void APlanetRushEnemy::RotateInMeteorAttack(float Value)
{
	UVParam.W = -Value;
	EarthMaterialInstanceDynamic->SetVectorParameterValue(TEXT("UVParam"), UVParam);
	CloudMaterialInstanceDynamic->SetVectorParameterValue(TEXT("UVParam"), UVParam);
}

void APlanetRushEnemy::StartMeteorAttackInternal()
{
	for (int i = 1; i <= 3; i++)
	{
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this
			, &APlanetRushEnemy::StartMeteorAttackGameMode, 1.f * i, false);
	}
}

void APlanetRushEnemy::StartMeteorAttackGameMode()
{
	if (!GetWorld())
	{
		return;
	}

	APlanetRushGameModeBase* GameMode = Cast<APlanetRushGameModeBase>
		(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->StartMeteorAttack(MeteorCountPerSecond);
	}
}	
#pragma endregion


#pragma region Moon Attack
void APlanetRushEnemy::StartMoonAttack()
{
	APlanetRushGameModeBase* GameMode = Cast<APlanetRushGameModeBase>
		(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->PlayRotateSound(MoonAttackTime);
	}

	FOnTimelineFloat UpdateFunction;
	FOnTimelineEvent TimelineFinished;
	UpdateFunction.BindUFunction(this, FName("RotateInMoonAttack"));
	TimelineFinished.BindUFunction(this, FName("OnEndMoonAttack"));

	TimelineComponent->AddInterpFloat(CurveFloat, UpdateFunction);
	TimelineComponent->SetTimelineFinishedFunc(TimelineFinished);
	TimelineComponent->SetTimelineLength(MoonAttackTime);
	TimelineComponent->SetLooping(false);
	TimelineComponent->PlayFromStart();

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this
		, &APlanetRushEnemy::StartMoonAttackInternal, 1.0f, false);
}

void APlanetRushEnemy::StartMoonAttackInternal()
{
	APlanetRushGameModeBase* GameMode = Cast<APlanetRushGameModeBase>
		(GetWorld()->GetAuthGameMode());
	if (!GameMode)
	{
		return;
	}

	for (int i = 0; i < 10; i++)
	{
		do
		{
			FVector CurrentLocation = GetActorLocation();
			FVector RandomDirection = FMath::VRand();
			FVector TargetLocation = CurrentLocation + RandomDirection * 10000.f;
			FVector ToTargetVector = TargetLocation - CurrentLocation;
			FVector Normal = ToTargetVector.GetSafeNormal();
			float Dot = FVector::DotProduct(Normal, GetActorForwardVector());

			if (TargetLocation.Z > CurrentLocation.Z
				&& TargetLocation.Z < GameMode->MainCamera->GetActorLocation().Z
				&& Dot >= 0)
			{
				TargetLocation.Z = GameMode->PlanetRushCharacter->GetActorLocation().Z;

				AMoon* Moon = GetWorld()->SpawnActor<AMoon>
					(MoonClass, TargetLocation, FRotator::ZeroRotator);
				GameMode->RegistMoon(Moon);
				break;
			}
		} while (true);
	}
}

void APlanetRushEnemy::OnEndMoonAttack()
{
	StartWait();
}

void APlanetRushEnemy::RotateInMoonAttack(float Value)
{
	UVParam.Z = -Value;
	EarthMaterialInstanceDynamic->SetVectorParameterValue(TEXT("UVParam"), UVParam);
	CloudMaterialInstanceDynamic->SetVectorParameterValue(TEXT("UVParam"), UVParam);
}
#pragma endregion


void APlanetRushEnemy::RotateForCutScene(float Value)
{
	UVParam.W = -Value;
	EarthMaterialInstanceDynamic->SetVectorParameterValue(TEXT("UVParam"), UVParam);
	CloudMaterialInstanceDynamic->SetVectorParameterValue(TEXT("UVParam"), UVParam);
}
