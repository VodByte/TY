#include "TY_BBT_FaceToLoc.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UTY_BBT_FaceToLoc::UTY_BBT_FaceToLoc()
{
	NodeName = FString("FaceToLoc");

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UTY_BBT_FaceToLoc, BlackboardKey));
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UTY_BBT_FaceToLoc, BlackboardKey), AActor::StaticClass());
}

EBTNodeResult::Type UTY_BBT_FaceToLoc::ExecuteTask(UBehaviorTreeComponent& OwnerComp
	, uint8* NodeMemory)
{
	InitiBBData(OwnerComp);

	FVector Loc = FVector::ZeroVector;
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		UObject* KeyVal = BBComp->GetValueAsObject(BlackboardKey.SelectedKeyName);
		if (!KeyVal) return EBTNodeResult::Failed;
		Loc = Cast<AActor>(KeyVal)->GetActorLocation();
	}
	else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		Loc = BBComp->GetValueAsVector(BlackboardKey.SelectedKeyName);
		if (Loc == FVector::ZeroVector) return EBTNodeResult::Failed;
	}

	FRotator NewRot = FRotationMatrix::MakeFromX(Loc - OwnerPawn->GetActorLocation()).Rotator();
	OwnerPawn->SetActorRotation(NewRot);
	return EBTNodeResult::Succeeded;
}