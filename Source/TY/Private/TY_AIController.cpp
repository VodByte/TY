#include "TY_AIController.h"
#include "TY_ShootComponent.h"
#include "TY_WingMan.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "BehaviorTree/BlackboardComponent.h"

#define INITI_KEY(x) const FName ATY_AIController::x##KeyName = FName(TEXT(#x));

INITI_KEY(HasFireLine);
INITI_KEY(TargetActor);
INITI_KEY(LastNoiseLoc);

ATY_AIController::ATY_AIController()
{
	PrimaryActorTick.bCanEverTick = true;

	// ~ Begin set up perception
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent")));

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
	// ~ End set up perception
}

void ATY_AIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);
	PlayerPtr = GetWorld()->GetFirstPlayerController<AController>()->GetPawn();
	check(PlayerPtr);
	OwnerPtr = GetPawn<ATY_WingMan>();
	check(OwnerPtr);
	OwnerPtr->OnTakeAnyDamage.AddDynamic(this, &ATY_AIController::HandleOnTakeAnyDamage);
}

void ATY_AIController::OnPawnDetected(const TArray<AActor*>& UpdatedActors)
{
	// UNKNOW BUG
	if (UpdatedActors.Num() == 0 || UpdatedActors.Num() > UpdatedActors.Max()) return;

	bool bCanSee = false;
	bool bCanHear = false;
	FActorPerceptionBlueprintInfo Info;
	for (auto& e : UpdatedActors)
	{
		GetPerceptionComponent()->GetActorsPerception(e, Info);
		bCanSee = Info.LastSensedStimuli[0].WasSuccessfullySensed();
		bCanHear = Info.LastSensedStimuli[1].WasSuccessfullySensed();
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
			LastNoiseLoc = Info.LastSensedStimuli[1].StimulusLocation;
			Blackboard->SetValueAsVector(LastNoiseLocKeyName, LastNoiseLoc);
		}
	}

	// Update BB
	if (SeeActors.Num() != 0)
	{
		for (auto& e : SeeActors)
		{
			if (e->Tags.Contains(FName("RedTeam")))
			{
				Blackboard->SetValueAsObject(TargetActorKeyName, e);
				break;
			}
		}
	}
	else
	{
		Blackboard->SetValueAsObject(TargetActorKeyName, nullptr);
	}

	if (!bCanHear) Blackboard->ClearValue(LastNoiseLocKeyName);
}

void ATY_AIController::HandleOnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType
	, AController* InstigatedBy, AActor* DamageCauser)
{
	// Update hatred info
	HatredMap.Emplace(DamageCauser);
	HatredMap[DamageCauser].HitCount++;
	if (HatredMap[DamageCauser].HitCount == 1)
	{
		HatredMap[DamageCauser].TimeStamp = GetWorld()->TimeSeconds;
	} 
}

void ATY_AIController::Tick(float DeltaTime)
{
	// Mark actors should be forgetten
	TSet<const AActor*> MarkedActor;
	if (HatredMap.Num() != 0)
	{
		for (auto& e : HatredMap)
		{
			float Diff = GetWorld()->TimeSeconds - e.Value.TimeStamp;
			if (Diff >= DamageAge)
			{
				MarkedActor.Add(e.Key);
			}
		}
	}

	// Forget actors
	if (MarkedActor.Num() != 0)
	{
		for (auto& e : MarkedActor)
		{
			HatredMap.Remove(e);
		}
	}
}