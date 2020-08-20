#pragma once

#include "CoreMinimal.h"
#include "TY_EnemyBBTBase.h"
#include "TY_BTT_FlyToLoc.generated.h"

UCLASS()
class TY_API UTY_BTT_FlyToLoc : public UTY_EnemyBBTBase
{
	GENERATED_BODY()

public:
	UTY_BTT_FlyToLoc();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector DestKey;

private:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp
		, uint8* NodeMemory);
};