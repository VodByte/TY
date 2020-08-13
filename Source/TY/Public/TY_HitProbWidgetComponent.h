#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "TY_HitProbWidgetComponent.generated.h"

UCLASS( ClassGroup=(TY), meta=(BlueprintSpawnableComponent) )
class TY_API UTY_HitProbWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UTY_HitProbWidgetComponent();
	virtual void InitWidget();

protected:

private:
};