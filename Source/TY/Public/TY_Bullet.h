// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TY_GunData.h"
#include "TY_Bullet.generated.h"

class UTY_BaseDamageType;

UCLASS(Abstract)
class TY_API ATY_Bullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATY_Bullet();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float BaseDamage = 10;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FName> TargetTags;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TSubclassOf<UTY_BaseDamageType> DamageType;

	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void InitiBullet(const FTY_GunData& InGunData, APawn* InBulletOwner) PURE_VIRTUAL(ATY_Bullet::InitBullet, );
};