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

void AObjectPool::PostInitializeComponents()
{
	AActor::PostInitializeComponents();

	Pools.Add(PoolName, this);
}

TMap<FString, AObjectPool*> AObjectPool::GetPoolsMap()
{
	return Pools;
}


void AObjectPool::BeginPlay()
{
	Super::BeginPlay();
	
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
		UE_LOG(LogTemp, Error, TEXT("Obj be requested but dequeue failed"));
		return nullptr;
	}
}

void AObjectPool::ReturnPoolObject(UInPoolObjectComponent* InComp)
{
	if (Pool.Enqueue(InComp->GetOwner()))
	{
		++PoolRemain;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Obj enqueue failed"));
	}
}

int AObjectPool::GetPoolSize()
{
	return PoolSize;
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
		PoolHelper->SetOwnershipPool(this);
		PoolHelper->SetPoolObjectActive(false);
		Pool.Enqueue(PoolableActor);
	}

	PoolRemain = PoolSize;
}

UInPoolObjectComponent* AObjectPool::GetPoolHelperComp(AActor* InActor) const
{
	auto temp = InActor->GetComponentByClass(UInPoolObjectComponent::StaticClass());
	return Cast<UInPoolObjectComponent>(temp);
}