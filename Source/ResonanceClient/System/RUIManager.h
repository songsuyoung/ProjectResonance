#pragma once

#include "CoreMinimal.h"
#include "System/RUIManagerBase.h"
#include "GameplayTagContainer.h"
#include "RUIManager.generated.h"

class URRootWindow;
class ARHUDBase;
class URCommonActivatableWidget;

UCLASS(Blueprintable)
class RESONANCECLIENT_API URUIManager : public URUIManagerBase
{
	GENERATED_BODY()
public:
	static URUIManager* Get(UObject* Context);
	
	virtual void Initialize(ARHUDBase* HUDBase, TSubclassOf<URRootWindow> RootWidgetClass) override;

protected:
	virtual URCommonActivatableWidget* OpenWindowInternalImpl(UClass* WidgetClass, FGameplayTag LayerTag) override;;
	
protected:
	
	UPROPERTY(Transient)
	TObjectPtr<URRootWindow> RootWidget;
	
	UPROPERTY(Transient)
	TObjectPtr<ARHUDBase> HUD;

	UPROPERTY(Transient)
	TArray<TObjectPtr<URCommonActivatableWidget>> ActiveWidgets;
};
