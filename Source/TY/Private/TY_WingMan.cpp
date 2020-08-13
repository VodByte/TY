#include "TY_WingMan.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "TY_HealthComponent.h"

ATY_WingMan::ATY_WingMan()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanFly = true;
	GetCharacterMovement()->MaxFlySpeed = DefaultMaxFlySpeed;

	HealthComponent = CreateDefaultSubobject<UTY_HealthComponent>(FName("HealthComponent"));
}

void ATY_WingMan::UpdateLeanAmount(float InTurnDelta)
{
	const float TargetLean = FMath::Clamp(InTurnDelta, -1.f, 1.f) * MaxLeanAmount;
	if (GetVelocity().Size() > 0.f)
	{
		CurrentLeanAmount = FMath::FInterpTo(CurrentLeanAmount, TargetLean, 
			GetWorld()->DeltaTimeSeconds, 2.f);
	}
	else
	{
		CurrentLeanAmount = FMath::FInterpTo(CurrentLeanAmount, 0.f,
			GetWorld()->DeltaTimeSeconds, 8.f);
	}
}