#include "TY_BTT_FindRandomLoc.h"
#include "DrawDebugHelpers.h"

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
		FVector TempDir = FMath::VRand();
		//TempDir *= FVector(1.f, 1.f, 0.f);
		TempDir.Normalize();
		check(OwnerPawn);
		return BaseLoc + TempDir * FMath::FRandRange(MinDistance, MaxDistance);
	};

	// Check validity
	auto CheckIfDestValid = [this](const FVector& InVect)->bool
	{
		FVector BoundOri;
		FVector BoundExt;
		OwnerPawn->GetActorBounds(true, BoundOri, BoundExt);

		FHitResult HitInfo;
		static const FName CapsuleTraceSingleName(TEXT("CapsuleTraceSingle"));
		FCollisionQueryParams Parmas(CapsuleTraceSingleName, false);
		Parmas.AddIgnoredActor(OwnerPawn);
		TArray<AActor*> 
		const bool bHit = GetWorld()->SweepSingleByChannel(HitInfo, BoundOri, InVect
			, FQuat::Identity, ECollisionChannel::ECC_Visibility
			, FCollisionShape::MakeCapsule(BoundExt), Parmas);
		DrawDebugCapsule(GetWorld(), InVect, BoundExt.X, BoundExt.Z
			, FQuat::Identity, FColor::Red, false, 0.1f);
		return !bHit;
	};

	FVector NewDest = GetTempDest();
	for (int32 i = 0; i < ValityCheckCount; i++)
	{
		if (CheckIfDestValid(NewDest)) break;
		else NewDest = GetTempDest();
	}

	// Update BB data
	if (CheckIfDestValid(NewDest))
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
