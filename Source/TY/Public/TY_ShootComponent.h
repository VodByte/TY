#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TY_ShootComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShootDel);

class ATY_ObjectPool;
struct FTY_GunData;

/*
	Basic bullet shooting function compoent,
	for other gun or bullet, override Shoot method.
*/
UCLASS( ClassGroup=(TY), meta=(BlueprintSpawnableComponent) )
class TY_API UTY_ShootComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTY_ShootComponent();
	void InitiComponent(const USceneComponent* InMuzzleComp
		, const FTY_GunData* const InGunData);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType
		, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void PressTrigger(const FVector& InAimLoc);
	UFUNCTION(BlueprintCallable)
	void ReleaseTrigger();

	// Some getters
	const FTY_GunData* GetGunData() const
	{ return OwnerGunData;}

	UFUNCTION(BlueprintCallable)
	float GetMaxEnergy() const;

	UFUNCTION(BlueprintCallable)
	float GetCurrentEnergy() const;

	UFUNCTION(BlueprintCallable)
	bool IsShooting() const;
	
	UFUNCTION(BlueprintCallable)
	float GetCurrentBulletScatter() const;

	UFUNCTION(BlueprintCallable)
	FVector GetBulletInitiVel() const;

	UFUNCTION(BlueprintCallable)
	FVector GetMuzzleLoc() const;

	UFUNCTION(BlueprintCallable)
	float CalcHitProb(const AActor* InTarActor) const;


	/// <summary>
	/// Get acture bullet location that affectted by inertial vel and gravity,
	/// defaultly use target loc sortted in owner.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	FVector GetActureBulletLoc();
	UFUNCTION(BlueprintCallable)
	FVector GetActureBulletLocWithParam(const FVector& InAimLoc);
	UFUNCTION(BlueprintCallable)
	FVector CalcAimLoc(FVector TargetLoc, FVector TargetVel, float InTolerance = 0.2f) const;
	
	// Events
	UPROPERTY(BlueprintAssignable)
	FShootDel OnPressTrigger;
	UPROPERTY(BlueprintAssignable)
	FShootDel OnReleaseTrigger;
	UPROPERTY(BlueprintAssignable)
	FShootDel OnFireBullet;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsShooting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnergyRecoveryDelay = 1.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnergyRecoverySpeed = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxEnergy = 100.f;
	
private:
	// Fire weapon methods
	FTimerManager* WorldTM;
	FTimerHandle FireTimer;
	FTimerHandle RecoveryEnergyTimer;
	UFUNCTION()
	void AutoFire();
	UFUNCTION()
	void BrustFire();
	UFUNCTION()
	void SemiFire();
	void RecoveryEnergy();
	
	float CurrentEnergy;
	float CurrentBulletScatter;
	FRotator CachedRot; // For calc bullet vel inherit from parent
	FRotator DeltaRot = FRotator::ZeroRotator;
	const FVector* CurrentAimLocPtr = nullptr;
	const USceneComponent* MuzzleComp;
	const FTY_GunData* OwnerGunData;
	ATY_ObjectPool* BulletPool = nullptr;

	// Spawn damageable bullet or harmless fake bullet base on bullet type
	UFUNCTION()
	void SpawnBullet();
};