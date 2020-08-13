#include "TY_ObjectPool.h"
#include "Engine/World.h"
#include "TY_InPoolObjectComponent.h"

TMap<FString, ATY_ObjectPool*> ATY_ObjectPool::Pools;

ATY_ObjectPool::ATY_ObjectPool()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATY_ObjectPool::PostInitializeComponents()
{
	AActor::PostInitializeComponents();

	Pools.Add(PoolName, this);
}

// Not functional until turn bCanEverTick on
void ATY_ObjectPool::Tick(float DeltaSeconds)
{
	UE_LOG(LogTemp, Error, TEXT("%d"), PoolRemain);
}

void ATY_ObjectPool::BeginPlay()
{
	Super::BeginPlay();
	
	InitPool();
}

AActor* ATY_ObjectPool::RequestPoolObject(const FTransform& InTransfrom, const AActor * InOwner)
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

void ATY_ObjectPool::ReturnPoolObject(UTY_InPoolObjectComponent* InComp)
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

int ATY_ObjectPool::GetPoolSize()
{
	return PoolSize;
}

void ATY_ObjectPool::InitPool()
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
	UE_LOG(LogTemp, Log, TEXT("Pool initialized."));
}

UTY_InPoolObjectComponent* ATY_ObjectPool::GetPoolHelperComp(AActor* InActor) const
{
	auto temp = InActor->GetComponentByClass(UTY_InPoolObjectComponent::StaticClass());
	return Cast<UTY_InPoolObjectComponent>(temp);
}