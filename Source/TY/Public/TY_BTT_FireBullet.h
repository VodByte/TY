#pragma once

#include "CoreMinimal.h"
#include "TY_EnemyBBTBase.h"
#include "TY_BTT_FireBullet.generated.h"

class ATY_AIController;

UCLASS()
class TY_API UTY_BTT_FireBullet : public UTY_EnemyBBTBase
{
	GENERATED_BODY()

public:

private:
	ATY_AIController* AICon;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp
		, uint8* NodeMemory);
};