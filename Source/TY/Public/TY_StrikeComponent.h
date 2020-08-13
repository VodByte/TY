#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TY_StrikeComponent.generated.h"

DECLARE_EVENT(UTY_StrikeComponent, FStrikeEvent)
DECLARE_EVENT_OneParam(UTY_StrikeComponent, FStrikeOneParamEvent, const AActor*)

class UCharacterMovementComponent;
class ACameraActor;
class ATY_WingMan;
class UAnimMontage;
class ACharacter;
class AController;

/*
	Notice acture damage is applied by anim notify!
*/
UCLASS( ClassGroup=(TY), meta=(BlueprintSpawnableComponent) )
class TY_API UTY_StrikeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float StrikeMaxFlySpeed = 8000.f;

	// Methods
	UTY_StrikeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType
		, FActorComponentTickFunction* ThisTickFunction) override;
	void InitiComponent(float InDefaultMaxFlySpeed, UCharacterMovementComponent* const InMoveCompRef);
	void LaunchStrike();
	void StopStrike();
	void BackOff();
	void ApplyDamageToTarget();
	void ReleaseTargetAcotr();

	// Getters
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsStriking() {return bIsStriking;}

	// Events
	FStrikeEvent OnBeginStrike;
	FStrikeEvent OnEndStrike;
	FStrikeOneParamEvent OnStrikeHitted;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StrikeDetectRange = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AniDistanceToTarget = 80.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotToTargetBackSpeed = 383.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExpectedAniAngle = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BlendCamTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StrikeBaseDamge = 90.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StrikeCoolingTime;	// In sec

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<EObjectTypeQuery>> QueryObjTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TargetTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ExecuteAnimMotage;

private:
	bool bIsStriking = false;
	bool bIsExecuting = false;
	bool bCanStrike = true;
	float DefautlMaxFlySpeed;
	ACameraActor* ExecuteCam;
	ACharacter* OwnerChar;
	UCharacterMovementComponent* OwnerMoveComp;
	AController* OwnerController;
	TArray<AActor*> HittedEnemies;

	void SetControllable(bool InNewAble);
};