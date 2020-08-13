#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TY_HitProbWidget.generated.h"

class ATY_PlayerController;
class UTextBlock;

UCLASS(Abstract, Blueprintable)
class TY_API UTY_HitProbWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	const AActor* OwnerActor;

	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATY_PlayerController* PlayerConRef = nullptr;
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* TextBlock_HitProb = nullptr;

private:
};