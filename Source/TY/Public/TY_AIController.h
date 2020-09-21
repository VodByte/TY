#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "TY_AIController.generated.h"

class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UAISenseConfig_Damage;
class ATY_WingMan;

USTRUCT()
struct FHatredInfo
{
	GENERATED_BODY()

	int HitCount = 0;
	float TimeStamp = 0.f;
};

UCLASS()
class TY_API ATY_AIController : public AAIController
{
	GENERATED_BODY()
	
public:
	static const FName HasFireLineKeyName;
	static const FName TargetActorKeyName;
	static const FName LastNoiseLocKeyName;

	ATY_AIController();
	virtual void Tick(float DeltaTime) override;

	void UpdateFlySpeed(float InPercent);

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

	virtual void BeginPlay() override;
	//virtual void OnPossess(APawn* InPawn) override;

private:
	APawn* PlayerPtr = nullptr;
	ATY_WingMan* OwnerPtr = nullptr;

	UAISenseConfig_Sight* SightConfig;
	UAISenseConfig_Hearing* HearingConfig;
	TSet<AActor*> SeeActors;
	FVector LastNoiseLoc = FVector::ZeroVector;
	
	TMap<const AActor*, FHatredInfo> HatredMap;
	float DamageAge = 5.f;

	float DefaultFlySpeed = 0.f;
	float PartolSpeed = 0.f;

	UFUNCTION()
	void OnPawnDetected(const TArray<AActor*>& UpdatedActors);

	UFUNCTION()
	void HandleOnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType
		, class AController* InstigatedBy, AActor* DamageCauser);
};