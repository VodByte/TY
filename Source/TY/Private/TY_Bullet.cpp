// Fill out your copyright notice in the Description page of Project Settings.


#include "TY_Bullet.h"
#include "TY_BaseDamageType.h"

// Sets default values
ATY_Bullet::ATY_Bullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageType = UTY_BaseDamageType::StaticClass();
}

// Called when the game starts or when spawned
void ATY_Bullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATY_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}