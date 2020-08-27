#include "TY_BTT_FlyToLoc.h"
#include "Engine/World.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UTY_BTT_FlyToLoc::UTY_BTT_FlyToLoc()
{
	NodeName = FString("FlyToLoc");
	bNotifyTick = true;
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UTY_BTT_FlyToLoc, BlackboardKey));
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UTY_BTT_FlyToLoc, BlackboardKey), AActor::StaticClass());
}

EBTNodeResult::Type UTY_BTT_FlyToLoc::ExecuteTask(UBehaviorTreeComponent& OwnerComp
	, uint8* NodeMemory)
{
	InitiBBData(OwnerComp);
	if (!BBComp->IsValidKey(BlackboardKey.GetSelectedKeyID()))
	{
		return EBTNodeResult::Failed;
	}

	DestLoc = FVector::ZeroVector;
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		UObject* TempTar = BBComp->GetValueAsObject(BlackboardKey.SelectedKeyName);
		AActor* Target = Cast<AActor>(TempTar);
		DestLoc = Target->GetActorLocation();
	}
	else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		DestLoc = BBComp->GetValueAsVector(BlackboardKey.SelectedKeyName);
 	}
	return EBTNodeResult::InProgress;
}

void UTY_BTT_FlyToLoc::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	MoveDir = (DestLoc - OwnerPawn->GetActorLocation()).GetSafeNormal();
	OwnerPawn->AddMovementInput(MoveDir);
	if (FVector::Dist(OwnerPawn->GetActorLocation(), DestLoc) <= AcceptableRadius)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}