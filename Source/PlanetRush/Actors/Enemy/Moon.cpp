// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/Enemy/Moon.h"

#include "Enemy/PlanetRushEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile/Projectile.h"
#include "GameMode/PlanetRushGameModeBase.h"

// Sets default values
AMoon::AMoon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MoonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MoonMesh"));
	RootComponent = MoonMesh;
}

// Called when the game starts or when spawned
void AMoon::BeginPlay()
{
	Super::BeginPlay();

	// 현재 레벨에 스폰되어 있는 PlanetRushEnemy 클래스의 액터 찾기
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld()
		, APlanetRushEnemy::StaticClass(), FoundActors);
	TargetLocation = FoundActors[0]->GetActorLocation();
}

// Called every frame
void AMoon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move to (0, 0, 0) in world space slowly
	FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation
		, DeltaTime, 0.2f);
	SetActorLocation(NewLocation);

	if (FVector::Distance(NewLocation, TargetLocation) <= 1000.0f)
	{
		APlanetRushGameModeBase* GameMode = Cast<APlanetRushGameModeBase>
			(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode)
		{
			GameMode->ShowGameOver();
		}
	};
}

// 충돌
void AMoon::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp
	, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	AProjectile* Projectile = Cast<AProjectile>(Other);
	if (Projectile)
	{
		APlanetRushGameModeBase* GameMode = Cast<APlanetRushGameModeBase>
			(UGameplayStatics::GetGameMode(GetWorld()));
		if (GameMode)
		{
			GameMode->UnRegistMoon(this);
			GameMode->CheckMoon();
		}

		Destroy();
	}
}