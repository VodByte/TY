#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TY_HealthComponent.generated.h"

//DECLARE_EVENT(UTY_HealthComponent, FHealthCompEvnet)
//DECLARE_EVENT_OneParam(UTY_HealthComponent, FHealthCompOneParamEvnet, const UDamageType*)

class ATY_WingMan;
class UAnimMontage;

UCLASS( ClassGroup=(TY), meta=(BlueprintSpawnableComponent) )
class TY_API UTY_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTY_HealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleOnTakeAnyDamage(AActor* DamagedActor,
		float Damage, const class UDamageType* DamageType
		, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HealthComponent")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent")
	float MaxHealth = 100.f;

	//FHealthCompEvnet OnHpChange;
	//FHealthCompOneParamEvnet OnHpZero;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* ExplosionDeathParEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* StunDamagedAnimMotage;

private:
	FTimerHandle StunTimer;
	FTimerDelegate StunTimerDel;
	UFUNCTION()
	void EndStun(ATY_WingMan* DamagedWM);
};