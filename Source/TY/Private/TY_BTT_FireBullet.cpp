#include "TY_BTT_FireBullet.h"
#include "TY_AIController.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "TY_ShootComponent.h"

EBTNodeResult::Type UTY_BTT_FireBullet::ExecuteTask(UBehaviorTreeComponent& OwnerComp
	, uint8* NodeMemory)
{
	InitiBBData(OwnerComp);

	UWorld* World = GetWorld();
	check(World);
	FTimerManager& TM = World->GetTimerManager();
	if (TM.IsTimerActive(AutoFireTimer)) return EBTNodeResult::Failed;

	if (!ShootCompPtr)
	{
		ShootCompPtr = OwnerPawn->FindComponentByClass<UTY_ShootComponent>();
		check(ShootCompPtr);
	}

	FVector TarLoc = BBComp->GetValueAsVector(FirePointKey.SelectedKeyName);
	if (!FirePointKey.IsSet()) return EBTNodeResult::Failed;

	ShootCompPtr->PressTrigger(TarLoc);
	TM.SetTimer(AutoFireTimer, this, &UTY_BTT_FireBullet::Ceasefire
		, AutoFireDuration + FMath::FRandRange(-1.f, 1.f) * AutoFireDurationVarity * AutoFireDuration);
	return EBTNodeResult::Succeeded;
}

void UTY_BTT_FireBullet::Ceasefire()
{
	ShootCompPtr->ReleaseTrigger();
}