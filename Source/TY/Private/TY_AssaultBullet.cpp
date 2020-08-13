// Fill out your copyright notice in the Description page of Project Settings.


#include "TY_AssaultBullet.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TY_InPoolObjectComponent.h"
#include "TY_NormalBulletDamageType.h"

// Sets default values
ATY_AssaultBullet::ATY_AssaultBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	DamageType = UTY_NormalBulletDamageType::StaticClass();

	// Setup components.
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	SphereCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	RootComponent = SphereCollider;
	
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BulletMesh->SetupAttachment(RootComponent);
	BulletMesh->CastShadow = false;

	PoolHelper = CreateDefaultSubobject<UTY_InPoolObjectComponent>(TEXT("PoolHelper"));

	// No Outline
	BulletMesh->bRenderCustomDepth = true;
	BulletMesh->CustomDepthStencilValue = 1;

	ParticleTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail"));
	ParticleTrail->bAllowRecycling = true;
	ParticleTrail->SetupAttachment(RootComponent);

	// Set LifeSpan with InPoolObjectComponent
	InitialLifeSpan = 0.f;

	// Active after be putted
	ParticleTrail->Activate(false);
	ParticleTrail->SetAutoActivate(false);

	// Bind function.
	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ATY_AssaultBullet::HandleOnComponentBeginOverlap);
}

void ATY_AssaultBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DropSpeed += Gravity * DeltaTime;
	FVector CurrentBulletVelocity = (InitiBulletVelocity + FVector::DownVector * DropSpeed) * DeltaTime;
	// Rot align to velocity
	SetActorRotation(CurrentBulletVelocity.Rotation());
	AddActorWorldOffset(CurrentBulletVelocity);
}

void ATY_AssaultBullet::InitiBullet(const FTY_GunData& InGunData/*, FVector InInheritVel*/)
{
	TargetTags = InGunData.TargetTags;
	InitiBulletVelocity = InGunData.BulletSpeed * GetActorForwardVector()/* + InInheritVel*/;
	Gravity = InGunData.BulletGravity;
	BaseDamage = InGunData.BulletBaseDamge;
	DropSpeed = 0.0f;
}

void ATY_AssaultBullet::HandleOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bool bIsMatched = false;
	for (auto& e : TargetTags)
	{
		if (OtherActor->Tags.Contains(e))
		{
			bIsMatched = true;
			break;
		}
	}
	if (!bIsMatched) return;

	UGameplayStatics::ApplyPointDamage(OtherActor, BaseDamage, GetActorLocation(), SweepResult, nullptr, nullptr, DamageType);
	PoolHelper->ReturnToPool();
}