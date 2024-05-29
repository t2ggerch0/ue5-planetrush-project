// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemy/Meteor.h"

// Sets default values
AMeteor::AMeteor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeteorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeteorMesh"));
	RootComponent = MeteorMesh;
}

// Called when the game starts or when spawned
void AMeteor::BeginPlay()
{
	InitialLifeSpan = 5.0f;
	Super::BeginPlay();
}

// Called every frame
void AMeteor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation();
	NewLocation.Z -= 50.0f;
	SetActorLocation(NewLocation);
}

