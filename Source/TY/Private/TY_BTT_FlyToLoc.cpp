#include "TY_BTT_FlyToLoc.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "AIController.h"

EBTNodeResult::Type UTY_BTT_FlyToLoc::ExecuteTask(UBehaviorTreeComponent& OwnerComp
	, uint8* NodeMemory)
{
	APawn* OwnerPtr = Cast<APawn>(OwnerComp.GetOwner());
	FVector Dest = UBTFunctionLibrary::GetBlackboardValueAsVector(this, DestKey);
	FVector OwnerLoc = OwnerComp.GetOwner()->GetActorLocation();
	if (Dest != PrevDest)
	{
		PrevDest == Dest;
		InitiDist = FVector::Dist(OwnerLoc, Dest);
	}

	// Translation
	FVector DirToDest = (Dest - OwnerLoc).GetSafeNormal();
	FVector InputVect = DirToDest + 
		(FVector::Dist(OwnerLoc, Dest) / InitiDist) * (FVector::ZeroVector - DirToDest);
	OwnerPtr->AddMovementInput(InputVect, InputValue);
	
	// Rotation
	// If is chasing target, face to target.Otherwise face to vel
	bool bIsChasing = UBTFunctionLibrary::GetBlackboardValueAsBool(this, IsChasingKey);
	FRotator NewRot = FRotationMatrix::MakeFromX(OwnerLoc - (bIsChasing ?
		GetWorld()->GetFirstPlayerController<AAIController>()->GetPawn()->GetActorLocation()
		: Dest)).Rotator();
	OwnerPtr->SetActorRotation(NewRot);

	return EBTNodeResult::Succeeded;
}