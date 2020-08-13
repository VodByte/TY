// Fill out your copyright notice in the Description page of Project Settings.


#include "TY_Heroine.h"
#include "Camera/CameraComponent.h"
#include "TY_ShootComponent.h"
#include "TY_StrikeComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ATY_Heroine::ATY_Heroine()
{
	PrimaryActorTick.bCanEverTick = false;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
	
	// Setup camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CamerBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeLocation(FVector(0.f, 130.f, 100.f));
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->CameraLagMaxDistance = 20.f;
	FollowCam = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCam"));
	FollowCam->SetupAttachment(CameraBoom);
	DefaultCameraLoc = FVector(0.f, 130.f, 100.f);
	StrikeCameraLoc = FVector(0.f, 0.f, 120.f);

	// Bind event
	StrikeComp->OnBeginStrike.AddUObject(this, &ATY_Heroine::SetCamerBoomStrikeLoc);
	StrikeComp->OnEndStrike.AddUObject(this, &ATY_Heroine::SetCamerBoomDefaultLoc);

	// Default tag
	Tags.Add(FName(TEXT("Player")));
	Tags.Add(FName(TEXT("RedTeam")));
}

void ATY_Heroine::SetCamerBoomDefaultLoc()
{
	CameraBoom->SetRelativeLocation(DefaultCameraLoc);
}

void ATY_Heroine::SetCamerBoomStrikeLoc()
{
	CameraBoom->SetRelativeLocation(StrikeCameraLoc);
}