#include "TY_BTT_FlyToLoc.h"
#include "Engine/World.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "DrawDebugHelpers.h"
#include "TY_AIController.h"

UTY_BTT_FlyToLoc::UTY_BTT_FlyToLoc()
{
	NodeName = FString("FlyToLoc");
	bNotifyTick = true;
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UTY_BTT_FlyToLoc, BlackboardKey));
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UTY_BTT_FlyToLoc, BlackboardKey), AActor::StaticClass());
}

// Get data from BB
EBTNodeResult::Type UTY_BTT_FlyToLoc::ExecuteTask(UBehaviorTreeComponent& OwnerComp
	, uint8* NodeMemory)
{
	InitiBBData(OwnerComp);
	if (!BBComp->IsValidKey(BlackboardKey.GetSelectedKeyID()))
	{
		return EBTNodeResult::Failed;
	}

	if (!TYAICon)
	{
		TYAICon = Cast<ATY_AIController>(OwnerCon);
		check(TYAICon);
	}

	TYAICon->UpdateFlySpeed(SpeedPercent);
	DestLoc = FVector::ZeroVector;
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		UObject* TempTar = BBComp->GetValueAsObject(BlackboardKey.SelectedKeyName);
		AActor* Target = Cast<AActor>(TempTar);
		DestLoc = Target->GetActorLocation();
	}
	else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		DestLoc = BBComp->GetValueAsVector(BlackboardKey.SelectedKeyName);
 	}
	return EBTNodeResult::InProgress;
}

void UTY_BTT_FlyToLoc::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	DrawDebugSphere(GetWorld(), DestLoc, 22.f, 10, FColor::Red, false);

	auto FindRouteLoc = [this](const FVector& InApproach, float ObstacleLength, FVector ObstacleLoc)->bool
	{
		FVector BoundOri;
		FVector BoundExt;
		OwnerPawn->GetActorBounds(true, BoundOri, BoundExt);
		float SearchCircleRadius = FMath::Max(BoundExt.X, BoundExt.Y) / 2.f;

		float DivideRadian = FMath::Acos(1.f 
			- FMath::Pow(SearchCircleRadius * 2.f + Gap, 2.f) 
			/ (2.f * FMath::Pow(ObstacleLength + Gap + SearchCircleRadius, 2.f)));
		float DivideAngle = FMath::RadiansToDegrees(DivideRadian);
		
		int32 Count = 360.f / DivideAngle;
		for (int32 i = 0; i < Count; i++)
		{
			//FVector SearchLoc = ObstacleLoc + 
			//	FRotationMatrix::MakeFromY(OwnerPawn->GetActorLocation()).GetUnitAxes(FVector(EAxis::X)) 
			//	* (ObstacleLength + );
		}
	};

	if (!IsPathObstacle(DestLoc, true))
	{
		MoveDir = (DestLoc - OwnerPawn->GetActorLocation()).GetSafeNormal();
		OwnerPawn->AddMovementInput(MoveDir);
	}
	else
	{

	}

	if (FVector::Dist(OwnerPawn->GetActorLocation(), DestLoc) <= AcceptableRadius)
	{
		TYAICon->UpdateFlySpeed(1.f);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}