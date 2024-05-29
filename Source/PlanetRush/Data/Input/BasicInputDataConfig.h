// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputMappingContext.h"
#include "InputAction.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BasicInputDataConfig.generated.h"

/**
 * 
 */
UCLASS()
class PLANETRUSH_API UBasicInputDataConfig : public UObject
{
	GENERATED_BODY()
	
public:
	UBasicInputDataConfig();

public:
	UInputMappingContext* InputMappingContext = nullptr;

public:
	UInputAction* Move = nullptr;
	UInputAction* Fire = nullptr;
	UInputAction* Replace = nullptr;
	UInputAction* Dilation = nullptr;
	UInputAction* ToTitle = nullptr;
};
