#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "TY_BaseDamageType.generated.h"

class UTY_HealthComponent;

#define TEST_BIT(Bitmask, Bit) (((Bitmask) & (1 << static_cast<uint32>(Bit))) > 0)
#define SET_BIT(Bitmask, Bit) (Bitmask |= 1 << static_cast<uint32>(Bit))
#define CLEAR_BIT(Bitmask, Bit) (Bitmask &= ~(1 << static_cast<uint32>(Bit)))

UENUM(meta=(Bitflags))
enum class EDamageDebuff
{
	Stun
};


UCLASS()
class TY_API UTY_BaseDamageType : public UDamageType
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Bitmask, BitmaskEnum=EDamageDebuff))
	int32 DamageDebuff = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseStunTime = 5.f;
};