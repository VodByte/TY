#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TY_EnemyBBTBase.generated.h"

class UBlackboardComponent;

UCLASS(abstract, NotBlueprintable)
class TY_API UTY_EnemyBBTBase : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

protected:
	UBlackboardComponent* BBComp = nullptr;
	AAIController* OwnerCon = nullptr;
	APawn* OwnerPawn = nullptr;

	void InitiBBData(UBehaviorTreeComponent& OwnerComp);
	bool IsPathObstacle(const FVector& InDest, bool bLogInfo = false) const;
	bool IsDestObstacle(const FVector& InDest) const;
};