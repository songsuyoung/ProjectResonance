#include "RHUD.h"

// UE

#include "RUIManager.h"
#include "UI/HUD/RHUDMain.h"

void ARHUD::Init()
{
	Super::Init();

	URUIManager* UIManager = URUIManager::Get(this);
	
	ensure(UIManager);
	
	UIManager->Initialize(this, RootWidgetClass);
	
	URHUDMain::OpenWindow(UIManager, URHUDMain::StaticClass());
}