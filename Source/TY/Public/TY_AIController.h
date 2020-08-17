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
	
	friend class UTY_BTT_FireBullet;

public:
	ATY_AIController();

	UFUNCTION()
	void OnPawnDetected(const TArray<AActor*>& UpdatedActors);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBehaviorTree* BehaviorTree;

	// ~Begin vars about Perception
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
	UAISenseConfig_Sight* SightConfig;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAISenseConfig_Hearing* HearingConfig;
	// ~End vars about Perception

	// ~Begin vars about formation
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AActor* PairPtr = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsWingMate = false;
	// ~End

	// ~Begin vars abhout atk
	float AutoFireDuration = 3.0f;
	// In Percent of origin value
	float AutoFireDurationVarity = 0.f;
	FTimerHandle AutoFireTimer;
	UTY_ShootComponent* ShootCompRef;

	virtual void BeginPlay() override;
	virtual void Fire();
	virtual void Ceasefire();

	//virtual void OnPossess(APawn* InPawn) override;
	//virtual void Tick(float DeltaTime) override;

private:
	APawn* PlayerPtr = nullptr;
	ATY_WingMan* OwnerPtr = nullptr;
	static const FName CanSeeHostileKeyName;
	static const FName CanOnlyHearHostileKeyName;
	static const FName InterestLocKeyName;
};
