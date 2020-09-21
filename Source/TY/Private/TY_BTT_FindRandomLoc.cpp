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
	auto GetTempDest = [this]()->FVector
	{
		FVector RandVect = FMath::VRand().GetSafeNormal() * FMath::FRandRange(MinDistance, MaxDistance);
		FVector TempDest = BaseLoc + RandVect; 
		TempDest.Z = GroundHeight + OffGroundDist + RandVect.Z;
		return TempDest;
	};

	FVector NewDest = GetTempDest();
	bool bDestValid = false;
	for (int32 i = 0; i < ValityCheckCount; i++)
	{
		if (IsDestObstacle(NewDest))
		{
			NewDest = GetTempDest();
		}
		else
		{
			bDestValid = true;
			break;
		}
	}

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