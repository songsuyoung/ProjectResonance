#include "System/RGameInstance.h"

// Newly Created File Names ..
#include "System/RDataManager.h"
#include "System/REventManager.h"

void URGameInstance::Init()
{
	Super::Init();

	check(DataManagerClass);

	DataManager = NewObject<URDataManager>(this, DataManagerClass);

	if (IsValid(DataManager))
	{
		DataManager->Initialize();
	}

	EventManager = NewObject<UREventManager>(this, UREventManager::StaticClass());
}
