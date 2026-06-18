#include "System/RUIManager.h"

#include "Blueprint/UserWidget.h"
#include "UI/System/RRootWindow.h"
#include "UI/RHUDBase.h"
#include "UI/System/RRootWindow.h"
URUIManager* URUIManager::Get(UObject* Context)
{
	return Cast<URUIManager>(URUIManagerBase::Get(Context));
}

void URUIManager::Initialize(ARHUDBase* HUDBase, TSubclassOf<URRootWindow> RootWidgetClass)
{
	RootWidget = CreateWidget<URRootWindow>(HUDBase->GetOwningPlayerController(), RootWidgetClass);

	if (IsValid(RootWidget))
	{
		RootWidget->AddToViewport();
	}
	
	HUD = HUDBase;
}

URCommonActivatableWidget* URUIManager::OpenWindowInternalImpl(UClass* WidgetClass, FGameplayTag LayerTag)
{
	URCommonActivatableWidget* CommonActivatableWidget = nullptr;
	
	if (IsValid(RootWidget))
	{
		CommonActivatableWidget = RootWidget->GetOrCreateInstance(WidgetClass, LayerTag);
		ActiveWidgets.Add(CommonActivatableWidget);
	}

	return CommonActivatableWidget;
}
