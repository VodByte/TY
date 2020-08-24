#include "..\Public\TY_EnemyBBTBase.h"
#include "TY_EnemyBBTBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"

UBlackboardComponent* UTY_EnemyBBTBase::BBComp = nullptr;
AAIController* UTY_EnemyBBTBase::OwnerCon = nullptr;
APawn* UTY_EnemyBBTBase::OwnerPawn= nullptr;

void UTY_EnemyBBTBase::InitiBBData(UBehaviorTreeComponent& OwnerComp)
{
	if (!OwnerCon)
	{
		OwnerCon = OwnerComp.GetAIOwner();
		OwnerPawn = OwnerCon->GetPawn();
		BBComp = OwnerCon->GetBlackboardComponent();
	}
}