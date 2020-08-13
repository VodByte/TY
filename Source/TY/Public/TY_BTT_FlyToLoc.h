#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TY_BTT_FlyToLoc.generated.h"

UCLASS()
class TY_API UTY_BTT_FlyToLoc : public UBTTaskNode
{
	GENERATED_BODY()

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp
			, uint8* NodeMemory);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FBlackboardKeySelector DestKey;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FBlackboardKeySelector IsChasingKey;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float InputValue = 20.f;

private:
	float InitiDist = 0;
	FVector PrevDest = FVector::ZeroVector;
};