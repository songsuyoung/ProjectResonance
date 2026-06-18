#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RUIManagerBase.generated.h"

class URCommonActivatableWidget;
class ARHUDBase;
class URRootWindow;

UCLASS()
class RESONANCE_API URUIManagerBase : public UObject
{
	GENERATED_BODY()
	
public:
	static URUIManagerBase* Get(UObject* Context);
	
	virtual void Initialize(ARHUDBase* HUDBase, TSubclassOf<URRootWindow> RootWidgetClass) { };
	URCommonActivatableWidget* OpenWindow(UClass* WidgeClass, FGameplayTag LayerTag) { return OpenWindowInternalImpl(WidgeClass, LayerTag); }
protected:
	virtual URCommonActivatableWidget* OpenWindowInternalImpl(UClass* WidgetClass, FGameplayTag LayerTag) { return nullptr; };
	
};
