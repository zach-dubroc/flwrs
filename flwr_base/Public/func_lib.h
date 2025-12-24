// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "func_lib.generated.h"

/**
 * 
 */
UCLASS()
class FLWR_BASE_API Ufunc_lib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static bool test_function(const int& input);
	
};
