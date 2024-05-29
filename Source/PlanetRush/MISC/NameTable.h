// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct PLANETRUSH_API FCollisionPresetNameTable
{
	static inline FName Player = TEXT("Player");
	static inline FName PlayerProjectile = TEXT("Projectile");
	static inline FName Enemy = TEXT("Enemy");
};

struct PLANETRUSH_API FAssetPathNameTable
{
	static inline const TCHAR* EnemyAIControllerBPClass
		= TEXT("/Game/PlanetRush/Blueprint/AI/BPAI_Enemy.BPAI_Enemy_C");
};