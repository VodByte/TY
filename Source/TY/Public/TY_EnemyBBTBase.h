#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TY_EnemyBBTBase.generated.h"

class UBlackboardComponent;

UCLASS()
class TY_API UTY_EnemyBBTBase : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	static UBlackboardComponent* BBComp;
	static AAIController* OwnerCon;
	static APawn* OwnerPawn;

	static const FName IsChasingKeyName;
	bool bIsChasing = false;
	static const FName InterestLocKeyName;
	FVector InterestLoc = FVector::ZeroVector;
	static const FName CanSeeHostileKeyName;
	bool bCanSeeHostile = false;
	static const FName CanOnlyHearHostileKeyName;
	bool bCanOnlyHearHostile = false;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp
		, uint8* NodeMemory, float DeltaSeconds) override;
};