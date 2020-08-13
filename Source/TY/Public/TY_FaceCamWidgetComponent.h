#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "TY_FaceCamWidgetComponent.generated.h"

UCLASS( ClassGroup=(TY), meta=(BlueprintSpawnableComponent) )
class TY_API UTY_FaceCamWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	virtual void InitWidget();
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType
		, FActorComponentTickFunction *ThisTickFunction) override;
};