#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "TY_AIController.generated.h"

class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UTY_ShootComponent;
class ATY_WingMan;

UCLASS()
class TY_API ATY_AIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ATY_AIController();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float AISightRadius = 4000.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float AISightAge = 5.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float AILoseSightRadius = AISightRadius + 1200.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float AIFieldOfView = 75.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float AIHearingRange = 7000.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float AIHearingAge = 5.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AActor* PairPtr = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsWingMate = false;

	virtual void BeginPlay() override;
	//virtual void OnPossess(APawn* InPawn) override;
	//virtual void Tick(float DeltaTime) override;

private:
	APawn* PlayerPtr = nullptr;
	ATY_WingMan* OwnerPtr = nullptr;

	UAISenseConfig_Sight* SightConfig;
	UAISenseConfig_Hearing* HearingConfig;
	TArray<AActor*> SeeActors;
	TArray<AActor*> HearActors;

	UFUNCTION()
	void OnPawnDetected(const TArray<AActor*>& UpdatedActors);
};
