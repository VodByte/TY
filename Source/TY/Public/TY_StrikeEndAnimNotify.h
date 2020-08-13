#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "TY_StrikeEndAnimNotify.generated.h"

UCLASS()
class TY_API UTY_StrikeEndAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};