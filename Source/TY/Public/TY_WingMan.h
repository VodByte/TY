#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TY_WingMan.generated.h"

class UTY_HealthComponent;
class UParticleSystem;

/*
	Basic flying character, only hold health component and
	info for animation BP
*/
UCLASS(abstract, NotBlueprintable)
class TY_API ATY_WingMan : public ACharacter
{
	GENERATED_BODY()

public:
	ATY_WingMan();

	// Calc lean info for setting currentLeanAmount var
	void UpdateLeanAmount(float InTurnDelta);

	FTimerHandle StunTimer;
	void BeginStun(float InStunTime);
	void EndStun();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTY_HealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultMaxFlySpeed = 2400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentLeanAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxLeanAmount = 10.f;
};