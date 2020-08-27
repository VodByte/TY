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
	UPROPERTY(EditAnywhere, meta=(ClampMin = "0.0", UIMin="0.0"))
	float AcceptableRadius = 5.f;

private:
	FVector MoveDir = FVector::ZeroVector;
	FVector DestLoc = FVector::ZeroVector;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp
		, uint8* NodeMemory);

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, 
		uint8* NodeMemory, float DeltaSeconds) override;
};