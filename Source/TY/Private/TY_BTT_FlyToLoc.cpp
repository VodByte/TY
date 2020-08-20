#include "TY_BTT_FlyToLoc.h"
#include "Engine/World.h"
#include "AIController.h"

UTY_BTT_FlyToLoc::UTY_BTT_FlyToLoc()
{
	NodeName = FString("FlyToLoc");
}

EBTNodeResult::Type UTY_BTT_FlyToLoc::ExecuteTask(UBehaviorTreeComponent& OwnerComp
	, uint8* NodeMemory)
{
	InitiBBData(OwnerComp);

	FVector OwnerLoc = OwnerPawn->GetActorLocation();
	FVector DestLoc = BBComp->GetValueAsVector(DestKey.SelectedKeyName);

	//if (InterestLoc != PrevInterestLoc)
	//{
	//	PrevInterestLoc == InterestLoc;
	//	InitiDist = FVector::Dist(OwnerLoc, InterestLoc);
	//}

	// Translation
	FVector DirToDest = (DestLoc - OwnerLoc).GetSafeNormal();
	OwnerPawn->AddMovementInput(DirToDest);
	//FVector InputVect = DirToDest + 
	//	(FVector::Dist(OwnerLoc, InterestLoc) / InitiDist) * (FVector::ZeroVector - DirToDest);
	//OwnerPawn->AddMovementInput(InputVect, InputValue);
	//
	//// Rotation
	//// If is chasing target, face to target.Otherwise face to vel
	//FRotator NewRot = FRotationMatrix::MakeFromX(OwnerLoc - (bIsChasing ?
	//	GetWorld()->GetFirstPlayerController<AAIController>()->GetPawn()->GetActorLocation()
	//	: InterestLoc)).Rotator();
	//OwnerPawn->SetActorRotation(NewRot);
	return EBTNodeResult::Succeeded;
}