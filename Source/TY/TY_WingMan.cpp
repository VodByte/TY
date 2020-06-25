#include "TY_WingMan.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ATY_WingMan::ATY_WingMan()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Set up marks for spring arm
	defaultCameraMark = CreateDefaultSubobject<USceneComponent>("TEXT(defaultCamearMark)");
	defaultCameraMark->SetupAttachment(RootComponent);
	strikeCameraMark = CreateDefaultSubobject<USceneComponent>("TEXT(strikeCameraMark)");
	strikeCameraMark->SetupAttachment(RootComponent);

	// Set up spring arm
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArmComp->bUsePawnControlRotation = true;
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(defaultCameraMark->GetRelativeLocation());

	// Create camera comp
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("normalCamera"));
	cameraComp->SetupAttachment(springArmComp, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void ATY_WingMan::BeginPlay()
{
	Super::BeginPlay();

	springArmComp->SetRelativeLocation(defaultCameraMark->GetRelativeLocation());
	currentEnergy = maxEnergy;
}

// Called every frame
void ATY_WingMan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Movement
	FRotator charYawRot(0.f, 0.f, GetControlRotation().Yaw);
	FVector forwardInput = FVector(charYawRot.RotateVector(FVector::ForwardVector)) *
		InputComponent->GetAxisValue("MoveForward");
	FVector rightInput = FVector(charYawRot.RotateVector(FVector::RightVector)) *
		GetInputAxisValue("MoveRight");
	FVector upInput = FVector(charYawRot.RotateVector(FVector::UpVector)) *
		GetInputAxisValue("MoveUp");
	FVector inputVec = (forwardInput + rightInput + upInput).GetSafeNormal();
	AddMovementInput(inputVec);

	if (isStriking) Strike(DeltaTime);
}

// Called to bind functionality to input
void ATY_WingMan::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Strike", IE_Pressed, this, &ATY_WingMan::TriggerStrike);
	PlayerInputComponent->BindAxis("MoveForward");
	PlayerInputComponent->BindAxis("MoveRight");
	PlayerInputComponent->BindAxis("MoveUp");
}

void ATY_WingMan::TriggerStrike()
{
	UE_LOG(LogTemp, Warning, TEXT("Hint"));
	isStriking = !isStriking;
	if (isStriking) springArmComp->SetRelativeLocation(strikeCameraMark->GetRelativeLocation());
	else springArmComp->SetRelativeLocation(defaultCameraMark->GetRelativeLocation());
}

void ATY_WingMan::Strike(float deltaTime)
{
	FVector curLocation = springArmComp->GetRelativeLocation();
	springArmComp->SetRelativeLocation(
		FMath::VInterpTo(curLocation, strikeSpringArmRelativeLocation, deltaTime
			, strikeSpringArmMoveSpeed));
}