#pragma once

#include "CoreMinimal.h"
#include "TY_WingMan.h"
#include "TY_GunData.h"
#include "TY_AssaultWing.generated.h"

class UTY_ShootComponent;
class UTY_StrikeComponent;
class UAnimMontage;

/*
	Character with assault gun and can strike other
 */
UCLASS()
class TY_API ATY_AssaultWing : public ATY_WingMan
{
	GENERATED_BODY()
	
public:
	ATY_AssaultWing();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* MuzzleLocMark;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTY_ShootComponent* ShootComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTY_StrikeComponent* StrikeComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTY_GunData AssaultGunData;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true,
		ClampMin=0.0, ClampMax=1.0, UIMin=0.0, UIMax=1.0))
	float BulletVelInhertialPrecent;
};