#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InPoolObjectComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TY_API UInPoolObjectComponent : public UActorComponent
{
	GENERATED_BODY()


	friend class AObjectPool;

public:	
	// Sets default values for this component's properties
	UInPoolObjectComponent();
	
	void SetPoolObjectActive(bool InActive, AActor* InActorOwner = nullptr);
	void ActivePoolObjectLifeTimer();
	FORCEINLINE void SetOwnershipPool(AObjectPool* InPool) { OwnershipPool = InPool; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	AObjectPool* OwnershipPool;
	AActor* Owner;
	float PoolObjectLifeSpan = 5.0f;
	FTimerHandle PoolObjectLifeSpanTimer;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,meta=(AllowPrivateAccess=true))
	bool PoolObjectActive = false;

	// Maybe not exit
	class UParticleSystemComponent* ParticleSysComp = nullptr;

	void ReturnToPool();
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