// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPool.h"
#include "Engine/World.h"
#include "InPoolObjectComponent.h"

TMap<FString, AObjectPool*> AObjectPool::Pools;

AObjectPool::AObjectPool()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

TMap<FString, AObjectPool*> AObjectPool::GetPoolsMap()
{
	return Pools;
}

void AObjectPool::BeginPlay()
{
	Super::BeginPlay();
	
	Pools.Add(PoolName, this);
	InitPool();
}

AActor* AObjectPool::RequestPoolObject(const FTransform& InTransfrom, AActor* InOwner)
{
	if (PoolRemain <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Pool dried"));
		return nullptr;
	}

	AActor* OutActor;
	if (Pool.Dequeue(OutActor))
	{
		--PoolRemain;
		auto PoolHelper = GetPoolHelperComp(OutActor);
		check(PoolHelper);
		PoolHelper->SetPoolObjectTransfrom(InTransfrom);
		PoolHelper->SetPoolObjectActive(true);
		return OutActor;
	}
	else
	{
		return nullptr;
	}
}

void AObjectPool::ReturnPoolObject(AActor* InActor)
{
	if (!InActor)
	{
		UE_LOG(LogTemp, Error, TEXT("Nullptr be returned in pool"));
		return;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Returned in pool"));
	}

	auto Helper = GetPoolHelperComp(InActor);
	Pool.Enqueue(InActor);
	++PoolRemain;
}

void AObjectPool::InitPool()
{
	UWorld* World = GetWorld();
	check(ObjectType && World);
	for (size_t i = 0; i < PoolSize; i++)
	{
		AActor* PoolableActor = World->SpawnActor<AActor>(ObjectType
			, FVector::ZeroVector, FRotator::ZeroRotator);
		auto PoolHelper = GetPoolHelperComp(PoolableActor);
		PoolHelper->SetOwnershipPool(PoolName);
		PoolHelper->DeactivePoolObject();
		Pool.Enqueue(PoolableActor);
	}

	PoolRemain = PoolSize;
}

UInPoolObjectComponent* AObjectPool::GetPoolHelperComp(AActor* InActor) const
{
	auto temp = InActor->GetComponentByClass(UInPoolObjectComponent::StaticClass());
	return Cast<UInPoolObjectComponent>(temp);
}