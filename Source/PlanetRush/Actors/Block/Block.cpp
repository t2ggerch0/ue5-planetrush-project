// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Block/Block.h"

// Sets default values
ABlock::ABlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh"));
	RootComponent = BlockMesh;
	BlockMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Create Dynamic Material
	BlockMaterial = BlockMesh->CreateDynamicMaterialInstance(0);
	BlockMesh->SetMaterial(0, BlockMaterial);
}

// Called when the game starts or when spawned
void ABlock::BeginPlay()
{
	Super::BeginPlay();

	APlanetRushGameModeBase* GameMode = Cast<APlanetRushGameModeBase>
		(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->RegistBlock(this);
	}
}

// Called every frame
void ABlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 ABlock::GetIdx() const
{
	return Idx;
}

void ABlock::CallMeteor()
{
	MoveBlock();

	FTimerHandle MoveSunTimerHandle;
	GetWorld()->GetTimerManager().SetTimer
	(MoveSunTimerHandle, this, &ABlock::SpawnMeteor, 0.95f, false);
}

void ABlock::MoveBlock()
{
	SetActiveBlock();

	FTimerHandle MoveBackTimerHandle;
	GetWorld()->GetTimerManager().SetTimer
	(MoveBackTimerHandle, this, &ABlock::SetInActiveBlock, 0.95f, false);
}

void ABlock::SetActiveBlock()
{
	FVector NewLocation = GetActorLocation();
	NewLocation.Z += 1;
	SetActorLocation(NewLocation);
}

void ABlock::SetInActiveBlock()
{
	FVector NewLocation = GetActorLocation();
	NewLocation.Z -= 1;
	SetActorLocation(NewLocation);
}

void ABlock::SpawnMeteor()
{
	FVector NewLocation = GetActorLocation();
	NewLocation.Z += 10000;
	AMeteor* Meteor = GetWorld()->SpawnActor<AMeteor>
		(MeteorClass, NewLocation, FRotator::ZeroRotator);
}

