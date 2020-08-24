#pragma once

#include "CoreMinimal.h"
#include "TY_EnemyBBTBase.h"
#include "TY_BBT_FaceToLoc.generated.h"

UCLASS()
class TY_API UTY_BBT_FaceToLoc : public UTY_EnemyBBTBase
{
	GENERATED_BODY()

public:
	UTY_BBT_FaceToLoc();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp
		, uint8* NodeMemory);
};