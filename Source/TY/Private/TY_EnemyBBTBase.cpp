#include "..\Public\TY_EnemyBBTBase.h"
#include "TY_EnemyBBTBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "DrawDebugHelpers.h"

void UTY_EnemyBBTBase::InitiBBData(UBehaviorTreeComponent& OwnerComp)
{
	if (!OwnerCon)
	{
		OwnerCon = OwnerComp.GetAIOwner();
		OwnerPawn = OwnerCon->GetPawn();
		BBComp = OwnerCon->GetBlackboardComponent();
	}
}

/// <summary>
/// Is there any obstacle in the path to dset?
/// </summary>
bool UTY_EnemyBBTBase::IsPathObstacle(const FVector& InDest, FHitResult* OutHitInfo) const
{
	FVector BoundOri;
	FVector BoundExt;
	OwnerPawn->GetActorBounds(true, BoundOri, BoundExt);

	static const FName CapsuleTraceSingleName(TEXT("CapsuleTraceSingle"));
	FCollisionQueryParams Parmas(CapsuleTraceSingleName, false);
	Parmas.AddIgnoredActor(OwnerPawn);
	// HACK: If don't reduce bound extent, when char stand on ground, ground will be
	// detected as obstracle.
	FHitResult HitInfo;
	const bool bHit = GetWorld()->SweepSingleByChannel(OutHitInfo ? *OutHitInfo : HitInfo, BoundOri, InDest
		, FQuat::Identity, ECollisionChannel::ECC_Visibility
		, FCollisionShape::MakeCapsule(BoundExt * 0.8f), Parmas);

	return bHit;
}

bool UTY_EnemyBBTBase::IsDestObstacle(const FVector& InDest) const
{
	FVector BoundOri;
	FVector BoundExt;
	OwnerPawn->GetActorBounds(true, BoundOri, BoundExt);

	const bool bOverlap = GetWorld()->OverlapAnyTestByChannel(InDest, FQuat::Identity
		, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeCapsule(BoundExt));
	return bOverlap;
}