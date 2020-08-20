#include "TY_AIController.h"
#include "TY_ShootComponent.h"
#include "TY_WingMan.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "BehaviorTree/BlackboardComponent.h"

ATY_AIController::ATY_AIController()
{
	PrimaryActorTick.bCanEverTick = false;

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent")));
	//BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	// Set up sight config
	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	// Set up hearing config
	HearingConfig->HearingRange = AIHearingRange;
	HearingConfig->SetMaxAge(AIHearingAge);
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &ATY_AIController::OnPawnDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
	GetPerceptionComponent()->ConfigureSense(*HearingConfig);
}

void ATY_AIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);
	PlayerPtr = GetWorld()->GetFirstPlayerController<AController>()->GetPawn();
	check(PlayerPtr);
	OwnerPtr = GetPawn<ATY_WingMan>();
	check(OwnerPtr);
}

void ATY_AIController::OnPawnDetected(const TArray<AActor*>& UpdatedActors)
{
	FActorPerceptionBlueprintInfo Info;
	for (auto e : UpdatedActors)
	{
		GetPerceptionComponent()->GetActorsPerception(e, Info);
		bool bCanSee = Info.LastSensedStimuli[0].WasSuccessfullySensed();
		bool bCanHear = Info.LastSensedStimuli[1].WasSuccessfullySensed();
		if (bCanSee)
		{
			SeeActors.Add(e);
		}
		else
		{
			if (SeeActors.Contains(e)) SeeActors.Remove(e);
		}
		
		if (bCanHear)
		{
			HearActors.Add(e);
		}
		else
		{
			if (HearActors.Contains(e)) HearActors.Remove(e);
		}
	}
}