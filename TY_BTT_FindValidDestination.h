#pragma once

#include "CoreMinimal.h"
#include "TY_EnemyBBTBase.h"
#include "TY_BTT_FindValidDestination.generated.h"

UCLASS()
class TY_API UTY_BTT_FindValidDestination : public UTY_EnemyBBTBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MinRoamDist = 500.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxRoamDist = 2000.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 CheckCount = 20;
	
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp
		, uint8* NodeMemory) override;

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp
			, uint8* NodeMemory) override;

	FVector StartLoc = FVector::ZeroVector;
};