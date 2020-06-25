#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TY_WingMan.generated.h"

UCLASS()
class TY_API ATY_WingMan : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* springArmComp = nullptr;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* cameraComp = nullptr;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		USceneComponent* defaultCameraMark = nullptr;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		USceneComponent*strikeCameraMark = nullptr;

	UPROPERTY(BlueprintGetter = GetIsStriking, VisibleAnywhere)
		bool isStriking = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float maxEnergy = 1200.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float currentEnergy = 1200.f;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float strikeMaxFlySpeed;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float defaultMaxFlySpeed;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float strikeMaxAcce;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float defaultMaxAcce;


	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FVector strikeSpringArmRelativeLocation {
		0.f, 130.f, 100.f
	};
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FVector defaultSpringArmRelativeLocation {};
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float strikeSpringArmMoveSpeed = 200.f;

public:
	// Sets default values for this character's properties
	ATY_WingMan();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void TriggerStrike();
	void Strike(float deltaTime);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintGetter)
		FORCEINLINE bool GetIsStriking() const { return isStriking; }
};