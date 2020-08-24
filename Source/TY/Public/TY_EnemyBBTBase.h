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
	static UBlackboardComponent* BBComp;
	static AAIController* OwnerCon;
	static APawn* OwnerPawn;

	void InitiBBData(UBehaviorTreeComponent& OwnerComp);
};