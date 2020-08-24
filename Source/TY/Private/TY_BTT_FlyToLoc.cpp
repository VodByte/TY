#include "TY_BTT_FlyToLoc.h"
#include "Engine/World.h"
#include "AIController.h"

UTY_BTT_FlyToLoc::UTY_BTT_FlyToLoc()
{
	NodeName = FString("FlyToLoc");
	bNotifyTick = true;
}

EBTNodeResult::Type UTY_BTT_FlyToLoc::ExecuteTask(UBehaviorTreeComponent& OwnerComp
	, uint8* NodeMemory)
{
	InitiBBData(OwnerComp);

	FVector OwnerLoc = OwnerPawn->GetActorLocation();
	UObject* TempTar = BBComp->GetValueAsObject(BlackboardKey.SelectedKeyName);
	check(TempTar);
	AActor* Target = Cast<AActor>(TempTar);
	DestLoc = Target->GetActorLocation();

	// Translation
	if (!DestLoc.IsNearlyZero())
	{
		return EBTNodeResult::InProgress;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong dest location"));
		return EBTNodeResult::Failed;
	}
}

void UTY_BTT_FlyToLoc::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FVector OwnerLoc = OwnerPawn->GetActorLocation();
	FVector DirToDest = (DestLoc - OwnerLoc).GetSafeNormal();
	OwnerPawn->AddMovementInput(DirToDest);

	if (FVector::Dist(OwnerLoc, DestLoc) <= AcceptableRadius)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UTY_BTT_FlyToLoc::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	DestLoc = FVector::ZeroVector;
}