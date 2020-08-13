#include "TY_StrikeComponent.h"
#include "GameFramework/Character.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "TY_StrikeDamageType.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "TY_PlayerController.h"
#include "Animation/AnimMontage.h"

UTY_StrikeComponent::UTY_StrikeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	OwnerChar = Cast<ACharacter>(GetOwner());
	QueryObjTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
}

void UTY_StrikeComponent::InitiComponent(float InDefaultMaxFlySpeed
	, UCharacterMovementComponent* const InMoveCompRef)
{
	DefautlMaxFlySpeed = InDefaultMaxFlySpeed;
	OwnerMoveComp = InMoveCompRef;
}


void UTY_StrikeComponent::BeginPlay()
{
	Super::BeginPlay();

	// Controller assigned after begin play
	OwnerController = OwnerChar->GetController();
	if (!ExecuteCam)
	{
		ExecuteCam = GetWorld()->SpawnActor<ACameraActor>(FVector::ZeroVector, FRotator::ZeroRotator);
	}
}

void UTY_StrikeComponent::SetControllable(bool InbNewAble)
{
	if (InbNewAble)
	{
		OwnerMoveComp->MovementMode = EMovementMode::MOVE_Flying;
		if (OwnerController->GetClass()->IsChildOf<APlayerController>())
		{
			auto PlayerCon = Cast<APlayerController>(OwnerController);
			PlayerCon->EnableInput(PlayerCon);
		}
		else
		{

		}
	}
	else
	{
		OwnerMoveComp->DisableMovement();
		if (OwnerController->GetClass()->IsChildOf<APlayerController>())
		{
			auto PlayerCon = Cast<APlayerController>(OwnerController);
			PlayerCon->DisableInput(PlayerCon);
		}
		else
		{

		}
	}
}

void UTY_StrikeComponent::TickComponent(float DeltaTime, ELevelTick TickType
	, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsStriking)
	{
		// Strike movment
		FVector InputVect = FRotationMatrix(OwnerChar->GetControlRotation()).GetUnitAxis(EAxis::X);
		OwnerMoveComp->AddInputVector(InputVect);

		// Detect
		bool Result = UKismetSystemLibrary::SphereOverlapActors(this, OwnerChar->GetActorLocation(), StrikeDetectRange
			, QueryObjTypes, nullptr, TArray<AActor*>{GetOwner()}, HittedEnemies);
		if (!Result) return;

		// Detected target! Break this loop
		if (!HittedEnemies[0]->Tags.Contains(TargetTag)) return;
		// UGameplayStatics::ApplyDamage(HittedEnemies[0], StrikeBaseDamge, nullptr, GetOwner(), UTY_StrikeDamageType::StaticClass());

		OnStrikeHitted.Broadcast(HittedEnemies[0]);
		OwnerMoveComp->StopMovementImmediately();
		OwnerChar->SetActorEnableCollision(false);
		HittedEnemies[0]->SetActorEnableCollision(false);
		// If owner if possessed by player controller, shut down input and movement
		
		bIsStriking = false;
		bIsExecuting = true;
	}
	else if (bIsExecuting)
	{
		auto Tar = HittedEnemies[0];
		FVector TarLoc = Tar->GetActorLocation();
		FVector TarUpVect = Tar->GetActorUpVector();
		FVector PlayerDir = FVector::VectorPlaneProject(OwnerChar->GetActorForwardVector(), TarUpVect);
		FVector TarDir =FVector::VectorPlaneProject(Tar->GetActorForwardVector(), TarUpVect);

		if (FMath::RadiansToDegrees(FMath::Acos(PlayerDir | TarDir)) > ExpectedAniAngle)
		{
			// Set new location
			FVector UnitDir = (OwnerChar->GetActorLocation() - TarLoc).GetSafeNormal();
			FVector DirToTargetBack = (FVector::VectorPlaneProject(UnitDir, TarUpVect)).GetSafeNormal();
			FVector NewLoc = DirToTargetBack.RotateAngleAxis(RotToTargetBackSpeed * DeltaTime, -TarUpVect) 
				* AniDistanceToTarget + TarLoc;
			OwnerChar->SetActorLocation(NewLoc);

			// Set new rotation
			FVector LookAtDir = TarLoc - OwnerChar->GetActorLocation();
			FRotator NewRot = FRotationMatrix::MakeFromX(LookAtDir).Rotator();
			OwnerChar->GetController()->SetControlRotation(NewRot);

		}
		else
		{
			// If reach loc, stop movement, blend to kill cam, execute target
			// Play execute ani
			OwnerChar->PlayAnimMontage(ExecuteAnimMotage);

			// Set execute cam location
			FVector MidLoc = (TarLoc + OwnerChar->GetActorLocation()) / 2.f;
			// Add height
			float HeightOffset = 44.f;
			float DistanceMultiplier = 111.f;
			float PitchMultiplier = 333.f;
			FVector CamTarLoc = FVector(0.f, 0.f, HeightOffset) + MidLoc;
			FVector CamLoc = CamTarLoc 
				+ (TarLoc - OwnerChar->GetActorLocation()).GetSafeNormal().RotateAngleAxis(90.f, FVector::UpVector) * DistanceMultiplier;
			FRotator CamRot = FRotationMatrix::MakeFromX(CamTarLoc - CamLoc).Rotator(); 
			CamRot = FRotator::MakeFromEuler(FVector(CamRot.Roll, CamRot.Pitch * PitchMultiplier, CamRot.Yaw));
			check(ExecuteCam);
			ExecuteCam->SetActorTransform(FTransform(CamRot, CamLoc));

			// If is player, set player controller view target
			auto PlayerControl = Cast<ATY_PlayerController>(OwnerChar->GetController());
			if (!PlayerControl) return;
			PlayerControl->SetViewTargetWithBlend(ExecuteCam, BlendCamTime);
			bIsExecuting = false;
		}
	}
}

void UTY_StrikeComponent::LaunchStrike()
{
	bIsStriking = true;
	bCanStrike = false;
	OwnerMoveComp->StopMovementImmediately();
	OwnerMoveComp->MaxFlySpeed = StrikeMaxFlySpeed;
	OwnerChar->bUseControllerRotationPitch = true;

	OnBeginStrike.Broadcast();
}

void UTY_StrikeComponent::StopStrike()
{
	bIsStriking = false;
	OwnerChar->SetActorEnableCollision(true);
	OwnerMoveComp->MaxFlySpeed = DefautlMaxFlySpeed;
	OwnerChar->bUseControllerRotationPitch = false;
	OwnerChar->SetActorRotation(FRotator::ZeroRotator);
	//OwnerChar->EnableInput(Cast<APlayerController>(OwnerController));
	
	// If is player's component reset camera
	auto PlayerControl = Cast<ATY_PlayerController>(OwnerChar->GetController());
	if (!PlayerControl) return;
	PlayerControl->SetViewTargetWithBlend(OwnerChar, BlendCamTime);

	OnEndStrike.Broadcast();
}

void UTY_StrikeComponent::BackOff()
{
	float BackOffMultiplier = 999.f;
	OwnerMoveComp->Velocity = (-OwnerChar->GetActorForwardVector()) * BackOffMultiplier;
}

void UTY_StrikeComponent::ApplyDamageToTarget()
{
	UGameplayStatics::ApplyDamage(HittedEnemies[0], StrikeBaseDamge, nullptr, GetOwner(), UTY_StrikeDamageType::StaticClass());
}

void UTY_StrikeComponent::ReleaseTargetAcotr()
{
	HittedEnemies[0]->SetActorEnableCollision(true);
}
