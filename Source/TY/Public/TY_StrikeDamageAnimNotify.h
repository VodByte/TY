// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "TY_StrikeDamageAnimNotify.generated.h"

UCLASS()
class TY_API UTY_StrikeDamageAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
private:  
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};