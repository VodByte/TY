#pragma once

#include "CoreMinimal.h"
#include "TY_Bullet.h"
#include "TY_AssaultBullet.generated.h"

class UParticleSystemComponent;
class USphereComponent;
class UTY_InPoolObjectComponent;

UCLASS()
class TY_API ATY_AssaultBullet : public ATY_Bullet
{
	GENERATED_BODY()

public:
	ATY_AssaultBullet();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category=AssaultBullet)
	virtual void InitiBullet(const FTY_GunData& InGunData, APawn* InBulletOwner) override;

protected:
	UFUNCTION()
	void HandleOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
		, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* BulletMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* ParticleTrail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* SphereCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTY_InPoolObjectComponent* PoolHelper;

private:
	UPROPERTY(VisibleAnywhere, Category=AssaultBullet)
	float Gravity = 1.f; 

	float DropSpeed = 0.f;
	FVector InitiBulletVelocity = FVector::ZeroVector;
	bool VelocityInitFlag = false;
};