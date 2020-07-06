#include "InPoolObjectComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Particles/ParticleSystemComponent.h"
#include "ObjectPool.h"

UInPoolObjectComponent::UInPoolObjectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UInPoolObjectComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	// Try to get particle component
	auto TriedActorComp = Owner->GetComponentByClass(UParticleSystemComponent::StaticClass());
	if (TriedActorComp) ParticleSysComp = Cast<UParticleSystemComponent>(TriedActorComp);
}

void UInPoolObjectComponent::SetPoolObjectLifeSpan(float LifeSpan_)
{
	PoolObjectLifeSpan = LifeSpan_;
	Owner->GetWorldTimerManager().SetTimer(PoolObjectLifeSpanTimer, this
		, &UInPoolObjectComponent::DeactivePoolObject, PoolObjectLifeSpan, false);
}

void UInPoolObjectComponent::SetPoolObjectActive(bool InActive, AActor* InActorOwner)
{
	PoolObjectActive = InActive;
	Owner->SetActorHiddenInGame(!InActive);
	Owner->SetActorEnableCollision(InActive);
	Owner->SetOwner(InActorOwner);
	if (InActive)
	{
		SetPoolObjectLifeSpan(PoolObjectLifeSpan);
	}
	else
	{
		Owner->GetWorldTimerManager().ClearTimer(PoolObjectLifeSpanTimer);
		auto Pool = AObjectPool::GetPoolsMap().Find(OwnershipPoolName);
		if (Pool) (*Pool)->ReturnPoolObject(Owner);
	}

	if (ParticleSysComp) ParticleSysComp->SetActive(InActive);
}