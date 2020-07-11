// Fill out your copyright notice in the Description page of Project Settings.


#include "AssaultBullet.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "InPoolObjectComponent.h"

// Sets default values
AAssaultBullet::AAssaultBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Setup components.
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	SphereCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	RootComponent = SphereCollider;
	
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BulletMesh->SetupAttachment(RootComponent);
	BulletMesh->CastShadow = false;

	PoolHelper = CreateDefaultSubobject<UInPoolObjectComponent>(TEXT("PoolHelper"));

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
	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &AAssaultBullet::HandleOnComponentBeginOverlap);
}

void AAssaultBullet::BeginPlay()
{
	Super::BeginPlay();
}

void AAssaultBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	BulletVelocity = GetActorForwardVector() * BulletSpeed * DeltaTime + FVector::DownVector * DropSpeed;
	// Rot align to velocity
	SetActorRotation(BulletVelocity.Rotation());
	AddActorWorldOffset(BulletVelocity);
}

FVector AAssaultBullet::GetBulletVelocity() const
{
	return BulletVelocity;
}

void AAssaultBullet::AddBulletVeloctiy(FVector InVel)
{
	BulletVelocity += InVel;
}

void AAssaultBullet::HandleOnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Bulelt overlapped, Tag is %s"), *TargetTag.ToString());
	if (!OtherActor->Tags.Contains(TargetTag)) return;
	
	UGameplayStatics::ApplyPointDamage(OtherActor, BaseDamage, GetActorLocation(), SweepResult, nullptr, nullptr, nullptr);
	PoolHelper->SetPoolObjectActive(false);
}