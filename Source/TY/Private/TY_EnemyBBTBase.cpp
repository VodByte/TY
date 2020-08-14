#include "TY_EnemyBBTBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBlackboardComponent* UTY_EnemyBBTBase::BBComp = nullptr;
AAIController* UTY_EnemyBBTBase::OwnerCon = nullptr;
APawn* UTY_EnemyBBTBase::OwnerPawn= nullptr;
const FName UTY_EnemyBBTBase::IsChasingKeyName = FName(TEXT("IsChasing"));
const FName UTY_EnemyBBTBase::InterestLocKeyName = FName(TEXT("InterestLoc"));
const FName UTY_EnemyBBTBase::CanSeeHostileKeyName = FName(TEXT("CanSeeHostile"));
const FName UTY_EnemyBBTBase::CanOnlyHearHostileKeyName = FName(TEXT("CanOnlyHearHostile"));

void UTY_EnemyBBTBase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!OwnerCon)
	{
		OwnerCon = OwnerComp.GetAIOwner();
		OwnerPawn = OwnerCon->GetPawn();
		BBComp = OwnerCon->GetBlackboardComponent();
	}

	bIsChasing = BBComp->GetValueAsBool(IsChasingKeyName);
	InterestLoc = BBComp->GetValueAsVector(InterestLocKeyName);
	bCanSeeHostile = BBComp->GetValueAsBool(CanSeeHostileKeyName);
	bCanOnlyHearHostile = BBComp->GetValueAsBool(CanOnlyHearHostileKeyName);
}