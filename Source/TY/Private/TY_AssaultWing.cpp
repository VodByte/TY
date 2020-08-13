#include "TY_AssaultWing.h"
#include "TY_ShootComponent.h"
#include "TY_StrikeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "TimerManager.h"

ATY_AssaultWing::ATY_AssaultWing()
{
	PrimaryActorTick.bCanEverTick = false;

	MuzzleLocMark = CreateDefaultSubobject<USceneComponent>(FName("MuzzleLocMark"));
	MuzzleLocMark->AttachToComponent(Cast<USceneComponent>(GetMesh()), 
		FAttachmentTransformRules::KeepRelativeTransform, FName("WeaponSocket"));
	ShootComp = CreateDefaultSubobject<UTY_ShootComponent>(FName("ShootComponent"));
	StrikeComp = CreateDefaultSubobject<UTY_StrikeComponent>(FName("StrikeComponent"));
}

void ATY_AssaultWing::BeginPlay()
{
	Super::BeginPlay();

	ShootComp->InitiComponent(MuzzleLocMark, &AssaultGunData);
	StrikeComp->InitiComponent(GetCharacterMovement()->MaxFlySpeed, GetCharacterMovement());
}