#pragma once

#include "CoreMinimal.h"
#include "TY_EnemyBBTBase.h"
#include "TY_BTT_FireBullet.generated.h"

class UTY_ShootComponent;

UCLASS()
class TY_API UTY_BTT_FireBullet : public UTY_EnemyBBTBase
{
	GENERATED_BODY()

public:
	UTY_BTT_FireBullet()
	{
		NodeName = FString("FireBullet");
	}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector FirePointKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AutoFireDuration = 3.0f;

	// In Percent of origin value
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AutoFireDurationVarity = 0.f;

private:
	FTimerHandle AutoFireTimer;
	UTY_ShootComponent* ShootCompPtr;

	void Ceasefire();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp
		, uint8* NodeMemory);
};