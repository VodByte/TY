#include "TY_AIController.h"
#include "TY_ShootComponent.h"
#include "TY_WingMan.h"
#include "TimerManager.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName ATY_AIController::CanSeeHostileKeyName = FName(TEXT("CanSeeHostile"));
const FName ATY_AIController::CanOnlyHearHostileKeyName = FName(TEXT("CanOnlyHearHostile"));
const FName ATY_AIController::InterestLocKeyName = FName(TEXT("InterestLoc"));

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
	// Find nearest hostile
	TArray<AActor*> HostileActors;
	for (auto& e : UpdatedActors)
	{
		if (e->Tags.Num() <= 0) continue;
		if (e->Tags.Contains("RedTeam"))
		{
			HostileActors.Add(e);
		}
	}

	auto PerComp = GetPerceptionComponent();
	bool bCanSeeHostile = false;
	bool bOnlyCanHearHostile = false;

	if (HostileActors.Num() == 0) return;
	// Sort hostile actor by distance to owner
	HostileActors.Sort([this](const AActor& A, const AActor& B)
		{
			auto SourceLoc = OwnerPtr->GetActorLocation();
			float DistA = FVector::DistSquared(SourceLoc, A.GetActorLocation());
			float DistB = FVector::DistSquared(SourceLoc, B.GetActorLocation());
			return DistA > DistB;
		});

	FActorPerceptionBlueprintInfo Info;
	for (auto e : HostileActors)
	{
		PerComp->GetActorsPerception(e, Info);
		bCanSeeHostile = Info.LastSensedStimuli[0].WasSuccessfullySensed();
		if (bCanSeeHostile)
		{
			break;
		}
	}

	// If cannot see any, check hearing
	if (!bCanSeeHostile)
	{
		for (auto e : HostileActors)
		{
			PerComp->GetActorsPerception(e, Info);
			bOnlyCanHearHostile = Info.LastSensedStimuli[1].WasSuccessfullySensed();
		}
	}
	
	Blackboard->SetValueAsBool(CanSeeHostileKeyName, bCanSeeHostile);
	Blackboard->SetValueAsBool(CanOnlyHearHostileKeyName, bOnlyCanHearHostile);

	// Update atk target loc
	if (bCanSeeHostile)
	{
		Blackboard->SetValueAsVector(InterestLocKeyName, PlayerPtr->GetActorLocation());
	}
}


void ATY_AIController::Fire()
{
	if (GetWorldTimerManager().IsTimerActive(AutoFireTimer)) return;

	if (!ShootCompRef)
	{
		ShootCompRef = GetPawn()->FindComponentByClass<UTY_ShootComponent>();
		check(ShootCompRef);
	}

	FVector TarLoc = Blackboard->GetValueAsVector(InterestLocKeyName);
	if (TarLoc.IsNearlyZero()) return;

	ShootCompRef->PressTrigger(TarLoc);
	GetWorldTimerManager().SetTimer(AutoFireTimer, this, &ATY_AIController::Ceasefire
		, AutoFireDuration + AutoFireDuration * FMath::FRandRange(-AutoFireDurationVarity, AutoFireDurationVarity));
}

void ATY_AIController::Ceasefire()
{
	ShootCompRef->ReleaseTrigger();
}