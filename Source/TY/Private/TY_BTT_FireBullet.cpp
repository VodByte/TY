#include "TY_BTT_FireBullet.h"
#include "TY_AIController.h"

EBTNodeResult::Type UTY_BTT_FireBullet::ExecuteTask(UBehaviorTreeComponent& OwnerComp
	, uint8* NodeMemory)
{
	if (!AICon)
	{
		AICon = Cast<ATY_AIController>(OwnerComp.GetAIOwner());
		check(AICon);
	}

	AICon->Fire();

	return EBTNodeResult::Succeeded;
}