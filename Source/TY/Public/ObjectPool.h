// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Queue.h"
#include "ObjectPool.generated.h"

class UInPoolObjectComponent;
class UParticleSystemComponent;

/*
* This Actor should be putted far away from main area.
* Because the issue of ribbon particle system may occur starnge behaviour.
*/
UCLASS()
class TY_API AObjectPool : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AObjectPool();

	static TMap<FString, AObjectPool*> Pools;

	UFUNCTION(BlueprintCallable)
	static TMap<FString, AObjectPool*> GetPoolsMap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	AActor* RequestPoolObject(const FTransform& InTransfrom, AActor* InOwner);

	UFUNCTION(BlueprintCallable)
	void ReturnPoolObject(AActor* InActor);

private:
	UPROPERTY(EditAnywhere, Category = ObjectPool)
	TSubclassOf<AActor> ObjectType;

	UPROPERTY(EditAnywhere, Category = ObjectPool)
	FString PoolName;
	
	UPROPERTY(EditAnywhere, Category = ObjectPool)
	int PoolSize;

	int PoolRemain = 0;
	TQueue<AActor*> Pool;

	void InitPool();
	UInPoolObjectComponent* GetPoolHelperComp(AActor* InActor) const;
};