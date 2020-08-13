#pragma once

#include "CoreMinimal.h"
#include "TY_GunData.generated.h"

UENUM()
enum EFireMode
{
	AutoMode,
	BurstMode,
	SemiMode,
};
UENUM()
enum EBulletType
{
	SolidBullet,
	RayBullet
};

USTRUCT(BlueprintType)
struct FTY_GunData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FullyAccuracyRange = 2700.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AccuracyLostRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GunFireRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BulletSpeed;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//float BulletInheritVelocityPercent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BulletGravity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BulletBaseDamge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BulletEnergyCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BulletScatterSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BulletDescatterSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BulletMaxScatterAngel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BulletPoolName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> TargetTags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EFireMode> FireMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EBulletType> BulletType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystem* MuzzleParticle;
};