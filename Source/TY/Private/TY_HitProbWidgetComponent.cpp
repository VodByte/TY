#include "TY_HitProbWidgetComponent.h"
#include "TY_HitProbWidget.h"

UTY_HitProbWidgetComponent::UTY_HitProbWidgetComponent()
{
	Space = EWidgetSpace::Screen;
}

void UTY_HitProbWidgetComponent::InitWidget()
{
	Super::InitWidget();

	auto HitProbWidget = Cast<UTY_HitProbWidget>(GetUserWidgetObject());
	if (!HitProbWidget) return;
	HitProbWidget->OwnerActor = GetOwner();
}