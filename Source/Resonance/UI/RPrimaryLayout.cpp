#include "UI/RPrimaryLayout.h"

// UE
#include "Components/TextBlock.h"

// New Created files...
#include "System/REventManager.h"
#include "System/RMessage.h"

void URPrimaryLayout::NativeConstruct()
{
	Super::NativeConstruct();

	UREventManager* EventManager = UREventManager::Get(this);

	check(EventManager);

	EventManager->AddReceiver(this);
}

void URPrimaryLayout::NativeDestruct()
{
	Super::NativeDestruct();

	UREventManager* EventManager = UREventManager::Get(this);

	check(EventManager);

	EventManager->RemoveReceiver(this);
}

void URPrimaryLayout::OnMessage(ERMessageType Type, FRMessage* Message)
{
	FRShowUIMessage* UIMessage = static_cast<FRShowUIMessage*>(Message);
}
