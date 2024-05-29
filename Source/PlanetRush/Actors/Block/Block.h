// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Enemy/Meteor.h"
#include "GameMode/PlanetRushGameModeBase.h"
#include "Components/StaticMeshComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Block.generated.h"

UCLASS()
class PLANETRUSH_API ABlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int32 GetIdx() const;
	void CallMeteor();
	void MoveBlock();
	void SetActiveBlock();
	void SetInActiveBlock();
	void SpawnMeteor();

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Block")
	UStaticMeshComponent* BlockMesh;

	UPROPERTY(EditAnywhere)
	UMaterialInstanceDynamic* BlockMaterial;

	UPROPERTY(EditAnywhere)
	int32 Idx;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AMeteor> MeteorClass;
};
