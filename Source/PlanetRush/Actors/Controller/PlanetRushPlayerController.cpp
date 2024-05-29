#include "Actors/Controller/PlanetRushPlayerController.h"

APlanetRushPlayerController::APlanetRushPlayerController()
{
	PlayerCameraManagerClass = APlanetRushPlayerCameraManager::StaticClass();
}

void APlanetRushPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APlanetRushPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	const UBasicInputDataConfig* BasicInputDataConfig = GetDefault<UBasicInputDataConfig>();

	Subsystem->AddMappingContext(BasicInputDataConfig->InputMappingContext, 0);
}

void APlanetRushPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent
		= Cast<UEnhancedInputComponent>(InputComponent);

	const UBasicInputDataConfig* BasicInputDataConfig
		= GetDefault<UBasicInputDataConfig>();

	EnhancedInputComponent->BindAction(BasicInputDataConfig->Move
		, ETriggerEvent::Triggered, this, &ThisClass::OnMove);

	EnhancedInputComponent->BindAction(BasicInputDataConfig->Fire
		, ETriggerEvent::Triggered, this, &ThisClass::OnFire);

	EnhancedInputComponent->BindAction(BasicInputDataConfig->Replace
		, ETriggerEvent::Triggered, this, &ThisClass::OnReplace);

	EnhancedInputComponent->BindAction(BasicInputDataConfig->Dilation
		, ETriggerEvent::Completed, this, &ThisClass::OnDilation);

	EnhancedInputComponent->BindAction(BasicInputDataConfig->ToTitle
		, ETriggerEvent::Completed, this, &ThisClass::OnToTitle);
}

void APlanetRushPlayerController::OnMove(const FInputActionValue& InputActionValue)
{
	const FVector InputAction = InputActionValue.Get<FVector>();
	APawn* ControlledPawn = GetPawn();
	ControlledPawn->AddMovementInput(FVector::ForwardVector, -InputAction.Y);
	ControlledPawn->AddMovementInput(FVector::RightVector, -InputAction.X);
}

void APlanetRushPlayerController::OnFire(const FInputActionValue& InputActionValue)
{
	Cast<APlanetRushCharacter>(GetPawn())->Fire();
}

void APlanetRushPlayerController::OnReplace(const FInputActionValue& InputActionValue)
{
	Cast<APlanetRushCharacter>(GetPawn())->Replace();
}

void APlanetRushPlayerController::OnDilation(const FInputActionValue& InputActionValue)
{
	if (UGameplayStatics::GetGlobalTimeDilation(GetWorld()) < 0.5f)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	}
	else
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1f);
	}
}

void APlanetRushPlayerController::OnToTitle(const FInputActionValue& InputActionValue)
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("TitleMap"));
}
