#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TY_PlayerController.generated.h"

class UTY_ShootComponent;
class UTY_StrikeComponent;
class UCharacterMovementComponent;
struct FTY_GunData;

/*
	For player
*/
UCLASS()
class TY_API ATY_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void TickActor(float DeltaTime, enum ELevelTick TickType
		, FActorTickFunction& ThisTickFunction) override;

	// Getters
	UFUNCTION(BlueprintCallable)
	bool IsAimingTarget() const;

	UFUNCTION(BlueprintCallable)
	float GetHitProb(const AActor* Tar) const;

	UFUNCTION(BlueprintCallable)
	float GetAimingTarHitProb() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TY)
	float StrikeTurnDivider = 44.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=TY)
	float StrikeMoveDivider = 2.0f;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// Input setup
	void MoveForward(float InValue);
	void MoveRight(float InValue);
	void MoveUp(float InValue);
	void Turn(float InValue);
	void LookUp(float InValue);
	void PressTrigger();
	void ReleaseTrigger();
	void SwitchStrike();
	void SwitchFlyPack();

	// Components ref
	UTY_ShootComponent* ShootComp;
	UTY_StrikeComponent* StrikeComp;
	UCharacterMovementComponent* MoveComp;

	bool bIsHoldTrigger = false;
	bool bIsAimingTar = false;
	FHitResult AimLineHit;
	FVector DefaultAimLoc;
	FVector ActureAimLoc;
};