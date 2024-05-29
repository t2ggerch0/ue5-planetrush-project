#pragma once

#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"


UENUM(BlueprintType)
enum class EProjectileType : uint8
{
	None,
	Fire = 0b0001,
	Water = 0b0010,
	Electric = 0b0100,
	End = 0b1000
};

USTRUCT()
struct PLANETRUSH_API FProjectileDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Type)
	EProjectileType ProjectileType;
	UPROPERTY(EditAnywhere, Category = Hit)
	float Damage;
	UPROPERTY(EditAnywhere, Category = Hit)
	UNiagaraSystem* EffectOnHit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hit)
	USoundBase* SoundOnHit;
	UPROPERTY(EditAnywhere, Category = StaticMeshComponent)
	UStaticMesh* StaticMesh;
	UPROPERTY(EditAnywhere, Category = StsaticMeshComponent)
	FTransform StaticMeshTransform;
	UPROPERTY(EditAnywhere, Category = StaticMeshComponent)
	UMaterial* StaticMeshMaterial;
	UPROPERTY(EditAnywhere, Category = SphereComponent)
	float ColliderSphereRadius = 32.f;
	UPROPERTY(EditAnywhere, Category = ProjectileMovementComponent)
	float ProjectileSpeed = 2000.f;
	UPROPERTY(EditAnywhere, Category = ProjectileMovementComponent)
	float ProjectileGravityScale = 0.f;
	UPROPERTY(EditAnywhere, Category = "Owner")
	float FireDelay = 1.f;
};

UCLASS()
class PLANETRUSH_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile();
	void SetProjectileData(const FProjectileDataTableRow* InData);

	UProjectileMovementComponent* GetMovementComponent() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit);

protected:
	UPROPERTY(EditAnywhere)
	FDataTableRowHandle DataTableRowHandle;

protected:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Collider;

	UPROPERTY(EditAnywhere)
	UArrowComponent* ArrowComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere)
	EProjectileType ProjectileType;

	UPROPERTY(VisibleAnywhere)
	float Damage;
	UPROPERTY(VisibleAnywhere)
	UNiagaraSystem* EffectOnHit;
	UPROPERTY(VisibleAnywhere)
	USoundBase* SoundOnHit;

};
