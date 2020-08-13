#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TY_BTT_FindValidDestination.generated.h"

UCLASS()
class TY_API UTY_BTT_FindValidDestination : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp
		, uint8* NodeMemory) override;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MinRoamDist = 500.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxRoamDist = 2000.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 CheckCount = 20;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FBlackboardKeySelector IsChasingKey;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FBlackboardKeySelector DestKey;

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp
			, uint8* NodeMemory) override;

	AActor* OwnerPtr = nullptr;
	FVector StartLoc = FVector::ZeroVector;
};