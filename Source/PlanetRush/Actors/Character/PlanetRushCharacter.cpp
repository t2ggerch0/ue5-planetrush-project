#include "Actors/Character/PlanetRushCharacter.h"
#include "Projectile/Projectile.h"
#include <Kismet/KismetMathLibrary.h>

APlanetRushCharacter::APlanetRushCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>
		(TEXT("CameraSpringArmComponent"));
	DefaultCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("DefaultCamera"));
	Head = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	GunSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>
		(TEXT("GunSpringArmComponent"));
	Gun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ZoomCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ZoomCamera"));

	USceneComponent* CurrentRootComponent = GetRootComponent();
	CameraSpringArmComponent->SetupAttachment(CurrentRootComponent);
	DefaultCamera->SetupAttachment(CameraSpringArmComponent);

	UMeshComponent* CurrentMeshComponent = GetMesh();
	Head->SetupAttachment(CurrentMeshComponent);
	Body->SetupAttachment(CurrentMeshComponent);
	GunSpringArmComponent->SetupAttachment(CurrentMeshComponent);
	Gun->SetupAttachment(GunSpringArmComponent);
	Muzzle->SetupAttachment(GunSpringArmComponent);
	Arrow->SetupAttachment(Muzzle);
	ZoomCamera->SetupAttachment(Muzzle);

	CameraSpringArmComponent->bUsePawnControlRotation = true;
	CameraSpringArmComponent->bInheritRoll = false;
	GunSpringArmComponent->bUsePawnControlRotation = true;
	GunSpringArmComponent->bInheritRoll = false;
}

void APlanetRushCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlanetRushCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlanetRushCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlanetRushCharacter::Fire()
{
	bool bTimer = GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle);
	if (bTimer) { return; }

	UDataSubsystem* DataSubsystem = GetWorld()->GetGameInstance()
		->GetSubsystem<UDataSubsystem>();
	const FProjectileDataTableRow* ProjectileRow
		= DataSubsystem->GetProjectile(CurrentProjectileType);

	ensure(ProjectileRow->FireDelay > 0.f);
	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, ProjectileRow->FireDelay, false);

	const FTransform& MuzzleTransform = Muzzle->GetComponentTransform();
	FTransform Transform = FTransform(MuzzleTransform.GetRotation(),
		MuzzleTransform.GetLocation());

	AProjectile* NewProjectile = GetWorld()->SpawnActorDeferred<AProjectile>(AProjectile::StaticClass(),
		Transform, this, this, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);


	NewProjectile->SetProjectileData(ProjectileRow);
	NewProjectile->FinishSpawning(MuzzleTransform, true);
}

void APlanetRushCharacter::Replace()
{
	CurrentProjectileType = (EProjectileType)((UINT16)CurrentProjectileType << 1)
		!= EProjectileType::End
		? (EProjectileType)((UINT16)CurrentProjectileType << 1)
		: EProjectileType::Fire;
}

void APlanetRushCharacter::SetMuzzleLookAt(FVector TargetLocation)
{
	FVector MuzzleLocation = Muzzle->GetComponentLocation();
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, TargetLocation);
	Muzzle->SetWorldRotation(LookAtRotation);
}

void APlanetRushCharacter::ShowGameOver()
{
	APlanetRushGameModeBase* GameMode = Cast<APlanetRushGameModeBase>
		(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->ShowGameOver();
	}
}

void APlanetRushCharacter::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other,
	UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation,
	FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation,
		HitNormal, NormalImpulse, Hit);
	if(Other->Tags.Contains("Plane"))
	{
		return;
	}

	if (OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_WorldStatic)
	{
		return;
	}

	SetActorHiddenInGame(true);
	ShowGameOver();
}