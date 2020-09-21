#pragma once

#include "CoreMinimal.h"
#include "TY_EnemyBBTBase.h"
#include "TY_BTT_FindRandomLoc.generated.h"

UCLASS()
class TY_API UTY_BTT_FindRandomLoc : public UTY_EnemyBBTBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GroundHeight = 880.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OffGroundDist = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDistance = 800.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinDistance = 200.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ValityCheckCount = 30;

	UTY_BTT_FindRandomLoc();

private:
	FVector BaseLoc = FVector::ZeroVector;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp
		, uint8* NodeMemory) override;

	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp
		, uint8* NodeMemory) override;
};