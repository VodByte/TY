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
	void SetPoolObjectLifeSpan(float LifeSpan_);
	FORCEINLINE void SetOwnershipPool(FString InString){ OwnershipPoolName = InString;}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	AActor* Owner;
	FString OwnershipPoolName = "Null";
	float PoolObjectLifeSpan = 5.0f;
	FTimerHandle PoolObjectLifeSpanTimer;
	bool PoolObjectActive = false;
	// Maybe not exit
	class UParticleSystemComponent* ParticleSysComp = nullptr;

	FORCEINLINE void DeactivePoolObject() { SetPoolObjectActive(false); }
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