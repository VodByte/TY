#include "TY_HealthComponent.h"
#include "TY_WingMan.h"
#include "TY_BaseDamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Animation/AnimMontage.h"

UTY_HealthComponent::UTY_HealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTY_HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	AActor* Ownner = GetOwner();
	if (Ownner)
	{
		Ownner->OnTakeAnyDamage.AddDynamic(this, &UTY_HealthComponent::HandleOnTakeAnyDamage);
	}
}

void UTY_HealthComponent::HandleOnTakeAnyDamage(AActor* DamagedActor,
	float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	float temp = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
	auto TYDamageType = Cast<UTY_BaseDamageType>(DamageType);
	check(TYDamageType);

	if (CurrentHealth != 0.f)
	{
		// Handle health changed
		if (temp == CurrentHealth) return;

		// Be hitted logic process
		if (TEST_BIT(TYDamageType->DamageDebuff, EDamageDebuff::Stun))
		{
			auto TYPawn = Cast<ATY_WingMan>(DamagedActor);
			TYPawn->TurnOff();
			TYPawn->SetActorEnableCollision(false);
			GetWorld()->GetTimerManager().ClearTimer(StunTimer);
			StunTimerDel.BindUFunction(this, FName("EndStun"), TYPawn);
			GetWorld()->GetTimerManager().SetTimer(StunTimer, StunTimerDel, TYDamageType->BaseStunTime, false);
			
			// HACK: Anim process : Stun anim = Strike ??????
			TYPawn->PlayAnimMontage(StunDamagedAnimMotage);
		}

	}
	else
	{
		// Handle death
		if (TEST_BIT(TYDamageType->DamageDebuff, EDamageDebuff::Stun))
		{

		}
		else
		{
			// Shoot death
			UE_LOG(LogTemp, Error, TEXT("%s dead"), *GetOwner()->GetName());
			UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionDeathParEffect
				, GetOwner()->GetActorLocation(), FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);
			GetOwner()->Destroy();
		}
	}
}

void UTY_HealthComponent::EndStun(ATY_WingMan* DamagedWM)
{
	DamagedWM->Reset();
	DamagedWM->SetActorEnableCollision(true);
}