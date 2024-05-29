// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectile/Projectile.h"
#include "MISC/MISC.h"

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InitialLifeSpan = 5.0f;

	// 컴포넌트 생성
	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(Collider);
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMeshComponent->SetupAttachment(Collider);
	ProjectileMovementComponent
		= CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	// 충돌
	{
		// 스피어 컴포넌트 있으니 스태틱 메시는 콜리전끄기
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// 콜리전 프리셋 네임 설정 및 이벤트 연결
		Collider->SetCollisionProfileName(FCollisionPresetNameTable::PlayerProjectile);
	}
}

UProjectileMovementComponent* AProjectile::GetMovementComponent() const
{
	return ProjectileMovementComponent;
}


void AProjectile::SetProjectileData(const FProjectileDataTableRow* InData)
{
	ensure(InData);

	Collider->SetSphereRadius(InData->ColliderSphereRadius);

	StaticMeshComponent->SetStaticMesh(InData->StaticMesh);
	StaticMeshComponent->SetRelativeTransform(InData->StaticMeshTransform);
	StaticMeshComponent->SetMaterial(0, InData->StaticMeshMaterial);

	ProjectileMovementComponent->MaxSpeed = InData->ProjectileSpeed;
	ProjectileMovementComponent->InitialSpeed = InData->ProjectileSpeed;
	ProjectileMovementComponent->ProjectileGravityScale = InData->ProjectileGravityScale;

	Damage = InData->Damage;
	EffectOnHit = InData->EffectOnHit;
	SoundOnHit = InData->SoundOnHit;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// NotifyHit
void AProjectile::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation,
	FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// 피격 사운드
	UGameplayStatics::PlaySoundAtLocation(this, SoundOnHit, HitLocation);

	// 피격 이펙트
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), EffectOnHit, HitLocation);

	Destroy();
}

