#include "TY_StrikeEndAnimNotify.h"
#include "Components/SkeletalMeshComponent.h"
#include "TY_StrikeComponent.h"

void UTY_StrikeEndAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	auto StrikeComp = MeshComp->GetOwner()->FindComponentByClass<UTY_StrikeComponent>();
	if (StrikeComp)
	{
		StrikeComp->StopStrike();
		StrikeComp->BackOff();
	}
	
}