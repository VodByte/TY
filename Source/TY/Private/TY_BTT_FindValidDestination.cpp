#include "TY_BTT_FindValidDestination.h"
#include "Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"

EBTNodeResult::Type UTY_BTT_FindValidDestination::ExecuteTask(UBehaviorTreeComponent& OwnerComp
	, uint8* NodeMemory)
{
	if (StartLoc.IsNearlyZero())
	{
		StartLoc = OwnerPawn->GetActorLocation();
	}

	// Get temp destination
	auto GetNewDest = [this]()->FVector
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
		OwnerPawn->GetActorBounds(true, BoundOri, BoundExt);

		FHitResult HitInfo;
		static const FName CapsuleTraceSingleName(TEXT("CapsuleTraceSingle"));
		FCollisionQueryParams Parmas(CapsuleTraceSingleName, false, OwnerPawn);
		const bool bHit = GetWorld()->SweepSingleByChannel(HitInfo, BoundOri, InVect
			, FQuat::Identity, ECollisionChannel::ECC_Visibility
			, FCollisionShape::MakeCapsule(BoundExt), Parmas);
		DrawDebugCapsule(GetWorld(), BoundOri, BoundExt.X, BoundExt.Z, FQuat::Identity, FColor::Red, false);
		return !bHit;
	};

	// Acturely calc and assignSS
	FVector Dest = FVector::ZeroVector;
	for (int32 i = 0; i < CheckCount; i++)
	{
		Dest = GetNewDest();
		if (CheckIfDestValid(Dest)) break;
		else Dest = FVector::ZeroVector;
	}
	BBComp->SetValueAsVector(InterestLocKeyName, Dest);

	if (Dest.IsNearlyZero())
	{
		UE_LOG(LogTemp, Error, TEXT("Node Failed, Find Valid Dest"));
		return EBTNodeResult::Failed;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Node Succeeded, Find Valid Dest"));
		return EBTNodeResult::Succeeded;
	}
}

EBTNodeResult::Type UTY_BTT_FindValidDestination::AbortTask(UBehaviorTreeComponent& OwnerComp
	, uint8* NodeMemory)
{
	StartLoc = FVector::ZeroVector;
	
	return EBTNodeResult::Aborted;
}