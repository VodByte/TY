#include "TY_BTT_FlyToLoc.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UTY_BTT_FlyToLoc::ExecuteTask(UBehaviorTreeComponent& OwnerComp
	, uint8* NodeMemory)
{
	RefreshBBData(OwnerComp);

	FVector OwnerLoc = OwnerPawn->GetActorLocation();
	if (InterestLoc != PrevInterestLoc)
	{
		PrevInterestLoc == InterestLoc;
		InitiDist = FVector::Dist(OwnerLoc, InterestLoc);
	}

	// Translation
	FVector DirToDest = (InterestLoc - OwnerLoc).GetSafeNormal();
	FVector InputVect = DirToDest + 
		(FVector::Dist(OwnerLoc, InterestLoc) / InitiDist) * (FVector::ZeroVector - DirToDest);
	OwnerPawn->AddMovementInput(InputVect, InputValue);
	
	// Rotation
	// If is chasing target, face to target.Otherwise face to vel
	FRotator NewRot = FRotationMatrix::MakeFromX(OwnerLoc - (bIsChasing ?
		GetWorld()->GetFirstPlayerController<AAIController>()->GetPawn()->GetActorLocation()
		: InterestLoc)).Rotator();
	OwnerPawn->SetActorRotation(NewRot);

	return EBTNodeResult::Succeeded;
}