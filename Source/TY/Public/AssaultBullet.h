#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AssaultBullet.generated.h"

class UParticleSystemComponent;
class USphereComponent;
class UInPoolObjectComponent;

UCLASS()
class TY_API AAssaultBullet : public AActor
{
	GENERATED_BODY()

public:
	AAssaultBullet();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TargetTag;

	UFUNCTION(BlueprintCallable, Category=AssaultBullet)
	FVector GetBulletVelocity() const;

	UFUNCTION(BlueprintCallable, Category=AssaultBullet)
	void AddBulletVeloctiy(FVector InVel);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
	UInPoolObjectComponent* PoolHelper;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AssaultBullet)
	float BaseDamage = 10;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	float BulletSpeed = 8000; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	float Gravity = 1.f; 

	float DropSpeed = 0.f;
	FVector BulletVelocity = FVector::ZeroVector;
	bool VelocityInitFlag = false;
};