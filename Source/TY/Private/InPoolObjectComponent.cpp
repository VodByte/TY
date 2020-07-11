#include "InPoolObjectComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Particles/ParticleSystemComponent.h"
#include "ObjectPool.h"

void UInPoolObjectComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	// Try to get particle component
	auto TriedActorComp = Owner->GetComponentByClass(UParticleSystemComponent::StaticClass());
	if (TriedActorComp) ParticleSysComp = Cast<UParticleSystemComponent>(TriedActorComp);
}

UInPoolObjectComponent::UInPoolObjectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInPoolObjectComponent::ActivePoolObjectLifeTimer()
{
	Owner->GetWorldTimerManager().SetTimer(PoolObjectLifeSpanTimer, this
		, &UInPoolObjectComponent::ReturnToPool, PoolObjectLifeSpan, false);
}

void UInPoolObjectComponent::SetPoolObjectActive(bool InActive, AActor* InActorOwner)
{
	PoolObjectActive = InActive;
	Owner->SetActorHiddenInGame(!InActive);
	Owner->SetActorEnableCollision(InActive);
	Owner->SetOwner(InActorOwner);
	if (InActive)
	{
		ActivePoolObjectLifeTimer();
	}
	else
	{
		Owner->GetWorldTimerManager().ClearTimer(PoolObjectLifeSpanTimer);
	}

	if (ParticleSysComp) ParticleSysComp->SetActive(InActive);
}

void UInPoolObjectComponent::ReturnToPool()
{
	SetPoolObjectActive(false);
	check(OwnershipPool);
	OwnershipPool->ReturnPoolObject(this);
	
}