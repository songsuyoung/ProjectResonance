#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "System/RUIManagerBase.h"
#include "UI/System/RCommonActivatableWidget.h"
#include "RHUDMain.generated.h"

UCLASS()
class RESONANCECLIENT_API URHUDMain : public URCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	static URHUDMain* OpenWindow(URUIManagerBase* UIManager, UClass* Class)
	{
		return Cast<URHUDMain>(UIManager->OpenWindow(Class, FGameplayTag::RequestGameplayTag(TEXT("UI.Layer.HUD"))));
	}
	
};
