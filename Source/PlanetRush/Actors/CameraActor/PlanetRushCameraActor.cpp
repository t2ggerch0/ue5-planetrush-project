// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CameraActor/PlanetRushCameraActor.h"
#include <Kismet/GameplayStatics.h>

APlanetRushCameraActor::APlanetRushCameraActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void APlanetRushCameraActor::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

void APlanetRushCameraActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector CameraLocation = GetActorLocation();
    FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    FVector TargetLocation = FVector(PlayerLocation.X, PlayerLocation.Y, CameraLocation.Z);
    FVector NewLocation = FMath::VInterpTo(CameraLocation, TargetLocation, DeltaTime, 10.0f);
    SetActorLocation(NewLocation);
}
