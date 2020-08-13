#include "TY_StrikeDamageAnimNotify.h"
#include "Components/SkeletalMeshComponent.h"
#include "TY_StrikeComponent.h"

void UTY_StrikeDamageAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)  
{  
	auto StrikeComp = MeshComp->GetOwner()->FindComponentByClass<UTY_StrikeComponent>();
	if (StrikeComp)
	{
		StrikeComp->ApplyDamageToTarget();
		StrikeComp->ReleaseTargetAcotr();
	}
}