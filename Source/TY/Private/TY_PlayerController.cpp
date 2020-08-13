#include "TY_PlayerController.h"
#include "TY_ShootComponent.h"
#include "TY_StrikeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "TY_GunData.h"

void ATY_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto ShootCompTemp = GetPawn<AActor>()->GetComponentByClass(UTY_ShootComponent::StaticClass());
	check(ShootCompTemp);
	ShootComp = Cast<UTY_ShootComponent>(ShootCompTemp);

	auto StrikeCompTemp = GetPawn<AActor>()->GetComponentByClass(UTY_StrikeComponent::StaticClass());
	check(StrikeCompTemp);
	StrikeComp = Cast<UTY_StrikeComponent>(StrikeCompTemp);

	auto MoveCompTemp = GetPawn<AActor>()->GetComponentByClass(UCharacterMovementComponent::StaticClass());
	check(MoveCompTemp);
	MoveComp = Cast<UCharacterMovementComponent>(MoveCompTemp);
}

void ATY_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("Turn", this, &ATY_PlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &ATY_PlayerController::LookUp);
	InputComponent->BindAxis("MoveUp", this, &ATY_PlayerController::MoveUp);
	InputComponent->BindAxis("MoveRight", this, &ATY_PlayerController::MoveRight);
	InputComponent->BindAxis("MoveForward", this, &ATY_PlayerController::MoveForward);
	
	InputComponent->BindAction("Shoot", IE_Pressed, this, &ATY_PlayerController::PressTrigger);
	InputComponent->BindAction("Shoot", IE_Released, this, &ATY_PlayerController::ReleaseTrigger);
	InputComponent->BindAction("SwitchStrike", IE_Pressed, this, &ATY_PlayerController::SwitchStrike);
	InputComponent->BindAction("SwitchFlyPack", IE_Pressed, this, &ATY_PlayerController::SwitchFlyPack);
}

void ATY_PlayerController::MoveForward(float InValue)
{
	if (StrikeComp->IsStriking()) return;

	const FRotator YawRot = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	const FVector MoveDir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
	MoveComp->AddInputVector(MoveDir * InValue);
}

void ATY_PlayerController::MoveRight(float InValue)
{
	if (StrikeComp->IsStriking()) InValue /= StrikeMoveDivider;

	const FRotator YawRot = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	const FVector MoveDir = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
	MoveComp->AddInputVector(MoveDir * InValue);
}

void ATY_PlayerController::MoveUp(float InValue)
{
	if (StrikeComp->IsStriking()) InValue /= StrikeTurnDivider;

	MoveComp->AddInputVector(FVector::UpVector * InValue);
}

void ATY_PlayerController::Turn(float InValue)
{
	if (StrikeComp->IsStriking()) InValue /= StrikeTurnDivider;

	AddYawInput(InValue);
}

void ATY_PlayerController::LookUp(float InValue)
{
	if (StrikeComp->IsStriking()) InValue /= StrikeTurnDivider;

	AddPitchInput(InValue);
}

void ATY_PlayerController::PressTrigger()
{
	bIsHoldTrigger = true;
	ShootComp->PressTrigger(ActureAimLoc);
}

void ATY_PlayerController::ReleaseTrigger()
{
	bIsHoldTrigger = false;
	ShootComp->ReleaseTrigger();
}

void ATY_PlayerController::SwitchStrike()
{
	if (StrikeComp->IsStriking())	StrikeComp->StopStrike();
	else							StrikeComp->LaunchStrike();
}

void ATY_PlayerController::SwitchFlyPack()
{
	bool bIsFlying = MoveComp->MovementMode == EMovementMode::MOVE_Flying;
	if (bIsFlying)
	{
		MoveComp->SetMovementMode(EMovementMode::MOVE_Falling);
	}
	else
	{
		MoveComp->SetMovementMode(EMovementMode::MOVE_Flying);
	}
}

void ATY_PlayerController::TickActor(float DeltaTime, ELevelTick TickType
	, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	auto World = GetWorld();
	auto GunData = ShootComp->GetGunData();
	check(World);
	// Aim detect
	auto CamManager = World->GetFirstPlayerController()->PlayerCameraManager;
	check(CamManager);
	FVector CamLoc = CamManager->GetCameraLocation();
	FVector CamForward = CamManager->GetActorForwardVector();
	DefaultAimLoc = CamLoc + CamForward
		* (GunData->FullyAccuracyRange + 100.f / GunData->AccuracyLostRate);

	bool bHitSome = World->LineTraceSingleByChannel(AimLineHit, CamLoc, DefaultAimLoc, ECC_Visibility);
	bIsAimingTar = false;
	if (bHitSome)
	{
		for (auto& e : GunData->TargetTags)
		{
			if (AimLineHit.Actor->Tags.Contains(e))
			{
				bIsAimingTar = true;
				break;
			}
		}
	}

	// Update aiming loc
	if (bIsHoldTrigger)
	{
		ActureAimLoc = DefaultAimLoc;
		if (bIsAimingTar)
		{
			auto Tar = AimLineHit.GetActor();
			check(Tar);
			bool bPerfectHit = FMath::FRand() <= ShootComp->CalcHitProb(Tar) / 100.f;
			if (bPerfectHit)
			{
				ActureAimLoc = ShootComp->CalcAimLoc(Tar->GetActorLocation(), Tar->GetVelocity());
			}
		}
	}
}

bool ATY_PlayerController::IsAimingTarget() const
{
	return bIsAimingTar;
}

float ATY_PlayerController::GetHitProb(const AActor* Tar) const
{
	return ShootComp->CalcHitProb(Tar);
}

float ATY_PlayerController::GetAimingTarHitProb() const
{
	if (!bIsAimingTar) return 0.f;
	else return GetHitProb(AimLineHit.GetActor());
}