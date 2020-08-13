#include "TY_FaceCamWidgetComponent.h"

void UTY_FaceCamWidgetComponent::InitWidget()
{
	Super::InitWidget();

	Space = EWidgetSpace::World;
}

void UTY_FaceCamWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType
	, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto World = GetWorld();
	if (!World) return;
	auto PlayCon =World->GetFirstPlayerController();
	if (!PlayCon) return;
	auto CamManager = PlayCon->PlayerCameraManager;
	FVector PointOnCamPlane = FVector::PointPlaneProject(GetComponentLocation(), CamManager->GetCameraLocation()
		, CamManager->GetActorForwardVector());
	FRotator NewRot = FRotationMatrix::MakeFromX(PointOnCamPlane - GetComponentLocation()).Rotator();
	SetWorldRotation(NewRot);
}