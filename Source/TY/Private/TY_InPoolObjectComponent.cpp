#include "TY_InPoolObjectComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Particles/ParticleSystemComponent.h"
#include "TY_ObjectPool.h"

void UTY_InPoolObjectComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	// Try to get particle component
	auto TriedActorComp = Owner->GetComponentByClass(UParticleSystemComponent::StaticClass());
	if (TriedActorComp) ParticleSysComp = Cast<UParticleSystemComponent>(TriedActorComp);
}

UTY_InPoolObjectComponent::UTY_InPoolObjectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTY_InPoolObjectComponent::ActivePoolObjectLifeTimer()
{
	Owner->GetWorldTimerManager().SetTimer(PoolObjectLifeSpanTimer, this
		, &UTY_InPoolObjectComponent::ReturnToPool, PoolObjectLifeSpan, false);
}

void UTY_InPoolObjectComponent::SetPoolObjectActive(bool InActive, AActor* InActorOwner)
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

void UTY_InPoolObjectComponent::ReturnToPool()
{
	SetPoolObjectActive(false);
	check(OwnershipPool);
	OwnershipPool->ReturnPoolObject(this);
}