#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Queue.h"
#include "TY_ObjectPool.generated.h"

class UTY_InPoolObjectComponent;
class UParticleSystemComponent;

/*
* This Actor should be putted far away from main area.
* Because the issue of ribbon particle system may occur starnge behaviour.
*/
UCLASS(ClassGroup = (TY))
class TY_API ATY_ObjectPool : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATY_ObjectPool();

	static TMap<FString, ATY_ObjectPool*> Pools;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	UFUNCTION(BlueprintCallable)
	AActor* RequestPoolObject(const FTransform& InTransfrom, const AActor* InOwner);

	UFUNCTION(BlueprintCallable)
	void ReturnPoolObject(UTY_InPoolObjectComponent* InActor);

	UFUNCTION(BlueprintCallable)
	int GetPoolSize();

private:
	UPROPERTY(EditAnywhere, Category = ObjectPool)
	TSubclassOf<AActor> ObjectType;

	UPROPERTY(EditAnywhere, Category = ObjectPool)
	FString PoolName;
	
	UPROPERTY(EditAnywhere, Category = ObjectPool)
	int PoolSize;

	int PoolRemain = 0;
	TQueue<AActor*, EQueueMode::Mpsc> Pool;

	void InitPool();
	UTY_InPoolObjectComponent* GetPoolHelperComp(AActor* InActor) const;
};