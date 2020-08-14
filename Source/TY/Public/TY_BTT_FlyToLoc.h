#pragma once

#include "CoreMinimal.h"
#include "TY_EnemyBBTBase.h"
#include "TY_BTT_FlyToLoc.generated.h"

UCLASS()
class TY_API UTY_BTT_FlyToLoc : public UTY_EnemyBBTBase
{
	GENERATED_BODY()

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp
			, uint8* NodeMemory);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float InputValue = 20.f;

private:
	float InitiDist = 0;
	FVector PrevInterestLoc = FVector::ZeroVector;
};