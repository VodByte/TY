#include "HealthComponent.h"
#include "GameFramework/Actor.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	DeafultHealth = 100.f;
}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = DeafultHealth;

	AActor* Ownner = GetOwner();
	if (Ownner)
	{
		Ownner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleOnTakeAnyDamage);
	}
}

void UHealthComponent::HandleOnTakeAnyDamage(AActor* DamagedActor,
	float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f) return;

	Health = FMath::Clamp(Health - Damage, 0.f, DeafultHealth);
	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(Health));
	if (Health == 0.f) OnHPZero.Broadcast(DamageType);
}