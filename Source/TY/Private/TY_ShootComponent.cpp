#include "TY_ShootComponent.h"
#include "TY_ObjectPool.h"
#include "TY_Bullet.h"
#include "TY_GunData.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

UTY_ShootComponent::UTY_ShootComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

FVector UTY_ShootComponent::GetActureBulletLoc()
{
	if ((*CurrentAimLocPtr).IsNearlyZero())
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong Aim loc for get acture bullet loc"));
		return FVector::ZeroVector;
	}
	return GetActureBulletLocWithParam(*CurrentAimLocPtr);
}

FVector UTY_ShootComponent::GetActureBulletLocWithParam(const FVector& InAimLoc)
{
	FVector AimDir = (InAimLoc - GetMuzzleLoc()).GetSafeNormal();
	FVector BulletVel = AimDir * OwnerGunData->BulletSpeed/* + CalcBulletInheritVel()*/;

	if (OwnerGunData->BulletGravity == 0.f)
	{
		float Time = FVector::Dist(GetMuzzleLoc(), InAimLoc)
			/ BulletVel.ProjectOnToNormal(AimDir).Size();
		return GetMuzzleLoc() + BulletVel * Time;
	}
	else
	{
		FVector BulletHoriVel = FVector::VectorPlaneProject(BulletVel, FVector::UpVector);
		float Time = FVector::VectorPlaneProject(InAimLoc - GetMuzzleLoc(), FVector::UpVector).Size()
			/ BulletHoriVel.Size();
		return GetMuzzleLoc() + BulletVel * Time 
			+ 0.5f * OwnerGunData->BulletGravity * FVector::DownVector * Time * Time;
	}
}

// https://www.wikiwand.com/en/Projectile_motion#/Angle_%7F'%22%60UNIQ--postMath-0000003A-QINU%60%22'%7F_required_to_hit_coordinate_/(x,y/)
FVector UTY_ShootComponent::CalcAimLoc(FVector TargetLoc, FVector TargetVel, float InTolerance) const
{
	FVector Dir = TargetLoc - GetMuzzleLoc();
	float BultSpeed = OwnerGunData->BulletSpeed;
	float G = OwnerGunData->BulletGravity;

	auto CalcHitLocWithNoG = [&]
	{
		float A = 1 - FMath::Pow(BultSpeed / TargetVel.Size(), 2.f);
		float Dist = Dir.Size();
		float CosTheta = TargetVel.CosineAngle2D(GetMuzzleLoc() - TargetLoc);
		float B = -(2.f * Dist * CosTheta);
		float C = Dist * Dist;

		float Delta = B * B - 4.f * A * C;
		if (Delta >= 0.f)
		{
			//float F1 = (-B + FMath::Sqrt(Delta)) / (2.f * A);
			float F2 = (-B - FMath::Sqrt(Delta)) / (2.f * A);
			float T = Dist / BultSpeed;
			//if (F1 < F2) F1 = F2;
			return TargetLoc + TargetVel.GetSafeNormal() * F2;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No Valid Root : %s ShootCompPtr::CalcAimLoc")
				, *GetOwner()->GetName());
			return FVector::ZeroVector;
		}
	};

	auto CalcAngleAndTime = [&](FVector PredictHitLoc)->FVector2D
	{
		FVector XVect = FVector::VectorPlaneProject(PredictHitLoc - GetMuzzleLoc(), FVector::UpVector);
		float X = XVect.Size();
		float Y = ((PredictHitLoc - GetMuzzleLoc()) - XVect).Size();
		
		float Delta = FMath::Pow(BultSpeed, 4.f)
			- G * (G * X * X + 2 * Y * BultSpeed * BultSpeed);
		if (Delta < 0.f) return FVector2D::ZeroVector;
		float Theta1 = FMath::Atan( (BultSpeed * BultSpeed - FMath::Sqrt(Delta)) / (G * X));
		float Theta2 = FMath::Atan( (BultSpeed * BultSpeed + FMath::Sqrt(Delta)) / (G * X));
		if (Theta1 > Theta2) Theta1 = Theta2;
		float T = X / (BultSpeed * FMath::Cos(Theta1));
		return FVector2D(Theta1, T);
	};

	if (G == 0.f)
	{
		return CalcHitLocWithNoG();
	}
	else
	{
		FVector OldPredictTargetLoc = TargetLoc;
		float OldDiff = 0.f;
		bool bIsDiffInitilized = false;
		float Angle;
		float T;
		
		while (1)
		{
			FVector2D Result = CalcAngleAndTime(OldPredictTargetLoc);
			if (Result.IsZero()) return FVector::ZeroVector;
			Angle = Result.X;
			T = Result.Y;

			FVector NewPredictTargetLoc = TargetLoc + TargetVel * T;
			float Diff = FVector::Dist(NewPredictTargetLoc, OldPredictTargetLoc);
			OldPredictTargetLoc = MoveTemp(NewPredictTargetLoc);
			if (!bIsDiffInitilized)
			{
				bIsDiffInitilized = true;
			}
			else
			{
				if (Diff > OldDiff) return FVector::ZeroVector;
				if (Diff <= InTolerance) break;
			}

			OldDiff = Diff;
		}

		return OldPredictTargetLoc + 0.5f * G * T * T * FVector::UpVector;
	}
}

float UTY_ShootComponent::CalcHitProb(const AActor* InTarActor) const
{
	if (!InTarActor) return 0.f;

	float LostPercent = (FVector::Dist(InTarActor->GetActorLocation(), GetMuzzleLoc())
		- OwnerGunData->FullyAccuracyRange) * OwnerGunData->AccuracyLostRate;
	return 100.f - FMath::Clamp(LostPercent, 0.f, 100.f);
}

void UTY_ShootComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentEnergy = MaxEnergy;
	WorldTM = &(GetWorld()->GetTimerManager());
	CachedRot = GetOwner()->GetActorRotation();
}

void UTY_ShootComponent::AutoFire()
{
	SpawnBullet();
	GetOwner()->MakeNoise(1.f, Cast<APawn>(GetOwner()), GetOwner()->GetActorLocation());
	CurrentEnergy = FMath::Clamp(CurrentEnergy - OwnerGunData->BulletEnergyCost, 0.f, MaxEnergy);
	if (CurrentEnergy == 0.f)
	{
		WorldTM->SetTimer(RecoveryEnergyTimer, this, &UTY_ShootComponent::RecoveryEnergy
			, 0.02f, true, EnergyRecoveryDelay);
	}
}

void UTY_ShootComponent::BrustFire()
{
}

void UTY_ShootComponent::SemiFire()
{
}

void UTY_ShootComponent::RecoveryEnergy()
{
	CurrentEnergy = FMath::Clamp(CurrentEnergy + EnergyRecoverySpeed, 0.f, MaxEnergy);
	if (CurrentEnergy == MaxEnergy) WorldTM->ClearTimer(RecoveryEnergyTimer);
}

void UTY_ShootComponent::InitiComponent(const USceneComponent* InMuzzleComp
	, const FTY_GunData* const InGunData)
{
	MuzzleComp = InMuzzleComp;
	OwnerGunData = InGunData;
	UE_LOG(LogTemp, Log, TEXT("ShootComponent initialized."));
}

void UTY_ShootComponent::TickComponent(float DeltaTime, ELevelTick TickType
	, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	DeltaRot = (GetOwner()->GetActorRotation() - CachedRot).GetNormalized();
	CachedRot = GetOwner()->GetActorRotation();

	// Cut down scatter angle
	if (CurrentBulletScatter != 0.f && !bIsShooting)
	{
		CurrentBulletScatter = FMath::Clamp(CurrentBulletScatter - OwnerGunData->BulletDescatterSpeed * DeltaTime
			, 0.f, OwnerGunData->BulletMaxScatterAngel);
	}
}

void UTY_ShootComponent::PressTrigger(const FVector& InAimLoc)
{
	CurrentAimLocPtr = &InAimLoc;

	// Beacuse of Pool initi after begin play, we cannot find pool before that
	if (BulletPool == nullptr)
	{
		auto PoolsTMap = ATY_ObjectPool::Pools;
		auto Pool = PoolsTMap.Find(OwnerGunData->BulletPoolName);
		BulletPool = *Pool;
	}

	switch (OwnerGunData->FireMode)
	{
		if (CurrentEnergy == 0.f || CurrentEnergy < OwnerGunData->BulletEnergyCost)
		{
			return;
		}

	case EFireMode::AutoMode:
		bIsShooting = true;
		if (WorldTM->IsTimerActive(RecoveryEnergyTimer))
		{
			WorldTM->ClearTimer(RecoveryEnergyTimer);
		}
		WorldTM->SetTimer(FireTimer, this
			, &UTY_ShootComponent::AutoFire, OwnerGunData->GunFireRate, true);
		break;
	case EFireMode::BurstMode:
		break;
	case EFireMode::SemiMode:
		break;
	default:
		checkNoEntry();
		break;
	}
}

void UTY_ShootComponent::ReleaseTrigger()
{
	switch (OwnerGunData->FireMode)
	{
	case EFireMode::AutoMode:
	{
		bIsShooting = false;
		WorldTM->ClearTimer(FireTimer);
		bool bIsRecoverying = WorldTM->IsTimerActive(RecoveryEnergyTimer);
		if (CurrentEnergy != MaxEnergy && !bIsRecoverying)
		{
			WorldTM->SetTimer(RecoveryEnergyTimer, this
				, &UTY_ShootComponent::RecoveryEnergy, 0.02f, true, EnergyRecoveryDelay);
		}
		break;
	}
	case EFireMode::BurstMode:
		break;
	case EFireMode::SemiMode:
		break;
	default:
		checkNoEntry();
		break;
	}
}

float UTY_ShootComponent::GetMaxEnergy() const
{
	return MaxEnergy;
}

float UTY_ShootComponent::GetCurrentEnergy() const
{
	return CurrentEnergy;
}

bool UTY_ShootComponent::IsShooting() const
{
	return bIsShooting;
}

float UTY_ShootComponent::GetCurrentBulletScatter() const
{
	return CurrentBulletScatter;
}

FVector UTY_ShootComponent::GetBulletInitiVel() const
{
	return OwnerGunData->BulletSpeed * (GetMuzzleLoc() - *CurrentAimLocPtr).GetSafeNormal();
}

FVector UTY_ShootComponent::GetMuzzleLoc() const
{
	return MuzzleComp->GetComponentLocation();
}

void UTY_ShootComponent::SpawnBullet()
{
	auto SpawnSolidBulelt = [this]
	{
		// Handle bullet scatter
		CurrentBulletScatter = FMath::Clamp(CurrentBulletScatter + OwnerGunData->BulletScatterSpeed
			, 0.f, OwnerGunData->BulletMaxScatterAngel);
		FVector RandPointInSphere = FVector(FMath::FRandRange(0.f, CurrentBulletScatter)
			, FMath::FRandRange(0.f, CurrentBulletScatter), FMath::FRandRange(0.f, CurrentBulletScatter));
		FQuat ScatterRot = FQuat::MakeFromEuler(RandPointInSphere);

		FVector Dir = *CurrentAimLocPtr - GetMuzzleLoc();
		FQuat BulletRot = FRotationMatrix::MakeFromX(Dir).ToQuat() * ScatterRot;
		AActor* BulletActor = BulletPool->RequestPoolObject(FTransform(BulletRot, GetMuzzleLoc()), GetOwner());
		check(BulletActor);
		auto Bullet = Cast<ATY_Bullet>(BulletActor);
		auto OwnerPawn = Cast<APawn>(GetOwner());
		check(OwnerPawn);
		Bullet->InitiBullet(*OwnerGunData, OwnerPawn);

		UGameplayStatics::SpawnEmitterAttached(OwnerGunData->MuzzleParticle, const_cast<USceneComponent*>(MuzzleComp), NAME_None
			, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true, EPSCPoolMethod::AutoRelease);
	};

	switch (OwnerGunData->BulletType)
	{
	case EBulletType::SolidBullet:
		OnFireBullet.Broadcast();
		SpawnSolidBulelt();
		break;
	default:
		checkNoEntry();
		break;
	}
}

//FVector UTY_ShootComponent::CalcBulletInheritVel() const
//{
//	// Add vel inherit
//	FVector InheritVel = GetOwner()->GetVelocity() * OwnerGunData->BulletInheritVelocityPercent;
//	// Add rotVel inherit
//	auto OwnerPawn = Cast<APawn>(GetOwner());
//	auto PlayerCon = Cast<APlayerController>(OwnerPawn->GetController());
//	float LookUpInputValue = DeltaRot.Pitch * PlayerCon->InputPitchScale * -1.f;
//	float TurnRightValue = DeltaRot.Yaw * PlayerCon->InputYawScale;
//	float MuzzleToOwnerDist = FVector::Dist(MuzzleComp->GetComponentLocation(), OwnerPawn->GetActorLocation());
//	FVector LookUpVel = OwnerPawn->GetActorUpVector() * MuzzleToOwnerDist * LookUpInputValue;
//	FVector TurnVel = OwnerPawn->GetActorRightVector() * MuzzleToOwnerDist * TurnRightValue;
//	return InheritVel += (LookUpVel + TurnVel);
//}