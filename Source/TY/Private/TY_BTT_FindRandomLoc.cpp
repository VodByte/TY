#include "TY_BTT_FindRandomLoc.h"

UTY_BTT_FindRandomLoc::UTY_BTT_FindRandomLoc()
{
	NodeName = FString("FindRandomLocation");

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UTY_BTT_FindRandomLoc, BlackboardKey));
}

EBTNodeResult::Type UTY_BTT_FindRandomLoc::ExecuteTask(UBehaviorTreeComponent& OwnerComp
	, uint8* NodeMemory)
{
	InitiBBData(OwnerComp);
	if (BaseLoc.IsNearlyZero())
	{
		BaseLoc = OwnerPawn->GetActorLocation();
	}

	// Get temp destination
	auto GetTempDest = [this](const FVector& InBaseLoc)->FVector
	{
		FVector TempDir = FMath::VRand();
		//TempDir *= FVector(1.f, 1.f, 0.f);
		TempDir.Normalize();
		check(OwnerPawn);
		return InBaseLoc + TempDir * FMath::FRandRange(MinDistance, MaxDistance);
	};

	auto LoopOutValidDest = [this, GetTempDest](const FVector& InBase, FVector& OutDest)->bool
	{
		for (int32 i = 0; i < ValityCheckCount; i++)
		{
			if (!IsPathObstacle(InBase)) return true;
			else OutDest = GetTempDest(InBase);
		}

		return false;
	};

	FVector NewDest = GetTempDest(BaseLoc);
	bool bDestValid = false;
	bDestValid = LoopOutValidDest(BaseLoc, NewDest);
	if (!bDestValid) bDestValid = LoopOutValidDest(OwnerPawn->GetActorLocation(), NewDest);

	if (bDestValid)
	{
		BBComp->SetValueAsVector(BlackboardKey.SelectedKeyName, NewDest);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		BBComp->ClearValue(BlackboardKey.SelectedKeyName);
		return EBTNodeResult::Failed;
	}
}

EBTNodeResult::Type UTY_BTT_FindRandomLoc::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BaseLoc = FVector::ZeroVector;
	return EBTNodeResult::Aborted;
} 