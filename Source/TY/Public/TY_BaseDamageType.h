// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "TY_BaseDamageType.generated.h"

/**
 * 
 */
UCLASS()
class TY_API UTY_BaseDamageType : public UDamageType
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TY_BaseDamage)
	float BaseDamage = 10;
};