// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Projectile/Projectile.h"

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataSubsystem.generated.h"

UCLASS()
class PLANETRUSH_API UDataSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    UDataSubsystem();
    const FProjectileDataTableRow* GetProjectile(const EProjectileType& ProjectilType);

private:
    TMap<EProjectileType, FProjectileDataTableRow*> TableRowByProjectileType;
};