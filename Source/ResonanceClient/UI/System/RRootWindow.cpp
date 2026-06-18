#include "UI/System/RRootWindow.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "RCommonActivatableWidget.h"
#include "RCommonActivatableWidgetStack.h"
#include "System/RUIWindowLoadMethod.h"

void URRootWindow::NativeConstruct()
{
	Super::NativeConstruct();
	
	TArray<UWidget*> ChildWidgets = Root->GetAllChildren();
	
	for (UWidget* ChildWidget : ChildWidgets)
	{
		UPanelWidget* Widget = Cast<UPanelWidget>(ChildWidget);
		
		if (IsValid(Widget))
		{
			URCommonActivatableWidgetStack* WidgetStack = Cast<URCommonActivatableWidgetStack>(Widget->GetChildAt(0));
			
			if (IsValid(WidgetStack))
			{
				LayerMap.Add(WidgetStack->GetLayerType(), WidgetStack);
			}
		}
	}
}

URCommonActivatableWidget* URRootWindow::GetOrCreateInstance(UClass* Class, const FGameplayTag& LayerTag)
{
	return GetOrCreateInstanceInternal(Class, LayerTag);
}

URCommonActivatableWidget* URRootWindow::GetOrCreateInstanceInternal(UClass* Class, const FGameplayTag& LayerTag)
{
	TObjectPtr<UCommonActivatableWidgetStack>* WidgetStack = LayerMap.Find(LayerTag);

	if (nullptr == WidgetStack)
	{
		return nullptr;
	}
	URCommonActivatableWidget* ReturnWidget = nullptr;

	FRPoolContainer* Pool = PoolingContainer.Find(Class->GetFName());

	if (nullptr != Pool)
	{
		int32 LastIndex = Pool->Container.Num() - 1;

		if (LastIndex >= 0)
		{
			//0보다 크면 마지막껄 꺼낸다.
			ReturnWidget = Pool->Container[LastIndex];
			Pool->Container.RemoveSingleSwap(ReturnWidget);
		}
	
		(*WidgetStack)->AddWidgetInstance(*ReturnWidget);
	}

	if (ReturnWidget == nullptr)
	{
		URUIWindowLoadMethod* WindowLoadMethodCDO = WindowLoadMethodClass->GetDefaultObject<URUIWindowLoadMethod>();

		if (false == IsValid(WindowLoadMethodCDO))
		{
			return nullptr;
		}

		TSubclassOf<URCommonActivatableWidget> ActivatableWidgetClass = WindowLoadMethodCDO->LoadAssetClass(Class);

		if (false == IsValid(ActivatableWidgetClass))
		{
			return nullptr;
		}

		ReturnWidget = Cast<URCommonActivatableWidget>((*WidgetStack)->AddWidget(ActivatableWidgetClass));
	}
	
	return ReturnWidget;
}

