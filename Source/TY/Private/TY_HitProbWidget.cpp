#include "TY_HitProbWidget.h"
#include "TY_ShootComponent.h"
#include "Components/TextBlock.h"
#include "Engine/World.h"
#include "TY_PlayerController.h"

void UTY_HitProbWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerConRef = GetWorld()->GetFirstPlayerController<ATY_PlayerController>();
	check(PlayerConRef);
}