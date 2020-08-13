// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TY_AssaultWing.h"
#include "TY_Heroine.generated.h"

class USpringArmComponent;
class UCameraComponent;

/*
	Player
*/
UCLASS()
class TY_API ATY_Heroine : public ATY_AssaultWing
{
	GENERATED_BODY()

	friend class ATY_PlayerController;
	
public:
	ATY_Heroine();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* FollowCam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category=TY)
	FVector DefaultCameraLoc = FVector(0.f, 130.f, 100.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category=TY)
	FVector StrikeCameraLoc = FVector(0.f, 0.f, 120.f);

private:
	UFUNCTION()
	void SetCamerBoomDefaultLoc();
	void SetCamerBoomStrikeLoc();
};