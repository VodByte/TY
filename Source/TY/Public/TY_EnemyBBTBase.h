#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TY_EnemyBBTBase.generated.h"

class UBlackboardComponent;

UCLASS(abstract, NotBlueprintable)
class TY_API UTY_EnemyBBTBase : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	static UBlackboardComponent* BBComp;
	static AAIController* OwnerCon;
	static APawn* OwnerPawn;

	//static const FName IsChasingKeyName;
	//static bool bIsChasing;
	//static const FName InterestLocKeyName;
	//static FVector InterestLoc;
	//static const FName CanSeeHostileKeyName;
	//static bool bCanSeeHostile;
	//static const FName CanOnlyHearHostileKeyName;
	//static bool bCanOnlyHearHostile;

	void InitiBBData(UBehaviorTreeComponent& OwnerComp);
};