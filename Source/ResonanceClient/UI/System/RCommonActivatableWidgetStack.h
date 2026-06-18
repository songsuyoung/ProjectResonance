#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "RCommonActivatableWidgetStack.generated.h"

UCLASS()
class RESONANCECLIENT_API URCommonActivatableWidgetStack : public UCommonActivatableWidgetStack
{
	GENERATED_BODY()
public:
	
	const FGameplayTag& GetLayerType() { return LayerTag; }
protected:
	
	UPROPERTY(EditAnywhere, Category = "Layer|Category")
	FGameplayTag LayerTag;
};
