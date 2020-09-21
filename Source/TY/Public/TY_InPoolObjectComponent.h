#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TY_InPoolObjectComponent.generated.h"

UCLASS( ClassGroup=(TY), meta=(BlueprintSpawnableComponent) )
class TY_API UTY_InPoolObjectComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class ATY_ObjectPool;

public:	
	// Sets default values for this component's properties
	UTY_InPoolObjectComponent();
	
	void ReturnToPool();
	void SetPoolObjectActive(bool InActive, AActor* InActorOwner = nullptr);
	void ActivePoolObjectLifeTimer();
	FORCEINLINE void SetOwnershipPool(ATY_ObjectPool* InPool) { OwnershipPool = InPool; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	ATY_ObjectPool* OwnershipPool;
	AActor* Owner;
	float PoolObjectLifeSpan = 5.0f;
	FTimerHandle PoolObjectLifeSpanTimer;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	bool PoolObjectActive = false;

	// Maybe not exit
	class UParticleSystemComponent* ParticleSysComp = nullptr;

	FORCEINLINE bool IsPoolObjectActive() const { return PoolObjectActive; }
	FORCEINLINE void SetPoolObjectTransfrom(const FTransform& InTransfrom)
	{
		//Owner->SetActorTransform(InTransfrom);	// If do this, scale will be mainpulted
		Owner->SetActorRotation(InTransfrom.GetRotation());
		Owner->SetActorLocation(InTransfrom.GetLocation());
		auto o = Owner->GetActorScale();
		auto n = InTransfrom.GetScale3D();
		Owner->SetActorScale3D(FVector(o.X*n.X, o.Y*n.Y, o.Z*n.Z));
	}
};