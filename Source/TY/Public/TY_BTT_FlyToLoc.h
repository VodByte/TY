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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0.0",ClampMax="1.0",UIMin="0.0",UIMax="1.0"))
	float SpeedPercent = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DetectDistance = 5.f;

private:
	//FVector MoveDir = FVector::ZeroVector;
	FVector DestLoc = FVector::ZeroVector;
	//FVector RouteLoc = FVector::ZeroVector;
	class ATY_AIController* TYAICon = nullptr;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp
		, uint8* NodeMemory);

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, 
		uint8* NodeMemory, float DeltaSeconds) override;
};