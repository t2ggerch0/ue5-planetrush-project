// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Input/BasicInputDataConfig.h"

UBasicInputDataConfig::UBasicInputDataConfig()
{
	{
		static ConstructorHelpers::FObjectFinder<UInputMappingContext> Asset
		{ TEXT("/Script/EnhancedInput.InputMappingContext'/Game/PlanetRush/Blueprint/Input/IMC_PlanetRush.IMC_PlanetRush'") };
		
		check(Asset.Succeeded());
		InputMappingContext = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/PlanetRush/Blueprint/Input/IA_Move.IA_Move'") };
		
		check(Asset.Succeeded());
		Move = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/PlanetRush/Blueprint/Input/IA_Fire.IA_Fire'") };

		check(Asset.Succeeded());
		Fire = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/PlanetRush/Blueprint/Input/IA_Replace.IA_Replace'") };

		check(Asset.Succeeded());
		Replace = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/PlanetRush/Blueprint/Input/IA_Dilation.IA_Dilation'") };

		check(Asset.Succeeded());
		Dilation = Asset.Object;
	}
	{
		static ConstructorHelpers::FObjectFinder<UInputAction> Asset
		{ TEXT("/Script/EnhancedInput.InputAction'/Game/PlanetRush/Blueprint/Input/IA_ToTitle.IA_ToTitle'") };

		check(Asset.Succeeded());
		ToTitle = Asset.Object;
	}
}
