#include "TY_BTT_FindValidDestination.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Engine/World.h"

EBTNodeResult::Type UTY_BTT_FindValidDestination::ExecuteTask(UBehaviorTreeComponent& OwnerComp
	, uint8* NodeMemory)
{
	if (!OwnerPtr)
	{
		OwnerPtr = OwnerComp.GetOwner();
		check(OwnerPtr);
	}

	if (StartLoc == FVector::ZeroVector)
	{
		StartLoc = OwnerPtr->GetActorLocation();
	}

	bool bIsChasing = UBTFunctionLibrary::GetBlackboardValueAsBool(this, IsChasingKey);

	// Get temp destination
	auto GetNewDest = [this, &bIsChasing]()->FVector
	{
		FVector TempDir = FMath::VRand();
		TempDir *= FVector(1.f, 1.f, 0.f);
		TempDir.Normalize();
		return (bIsChasing ? FVector::ZeroVector : StartLoc) 
			+ TempDir * FMath::FRandRange(MinRoamDist, MaxRoamDist);
	};

	// Check validity
	auto CheckIfDestValid = [this](const FVector& InVect)->bool
	{
		FVector BoundOri;
		FVector BoundExt;
		OwnerPtr->GetActorBounds(true, BoundOri, BoundExt);

		FHitResult HitInfo;
		static const FName CapsuleTraceSingleName(TEXT("CapsuleTraceSingle"));
		FCollisionQueryParams Parmas(CapsuleTraceSingleName, false, OwnerPtr);
		const bool bHit = GetWorld()->SweepSingleByChannel(HitInfo, BoundOri, InVect
			, FQuat::Identity, ECollisionChannel::ECC_Visibility
			, FCollisionShape::MakeCapsule(BoundExt), Parmas);

		return !bHit;
	};

	// Acturely calc and assign
	FVector Dest = FVector::ZeroVector;
	for (int32 i = 0; i < CheckCount; i++)
	{
		Dest = GetNewDest();
		if (CheckIfDestValid(Dest)) break;
		else Dest = FVector::ZeroVector;
	}
	UBTFunctionLibrary::SetBlackboardValueAsVector(this, DestKey, Dest);

	if (Dest.IsNearlyZero()) return EBTNodeResult::Failed;
	else return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UTY_BTT_FindValidDestination::AbortTask(UBehaviorTreeComponent& OwnerComp
	, uint8* NodeMemory)
{
	StartLoc = FVector::ZeroVector;
	
	return EBTNodeResult::Aborted;
}