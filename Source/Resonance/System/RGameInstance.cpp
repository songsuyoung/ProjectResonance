#include "System/RGameInstance.h"

// Newly Created File Names ..
#include "RWayPointManager.h"
#include "System/RDataManager.h"
#include "System/REventManager.h"

void URGameInstance::Init()
{
	Super::Init();

	check(DataManagerClass);

	DataManager = NewObject<URDataManager>(this, DataManagerClass);

	// UObject인 Subsystem의 Initialize 를 합칠 Interface 구축 또는 UObject 구축 
	if (IsValid(DataManager))
	{
		DataManager->Initialize();
	}

	EventManager = NewObject<UREventManager>(this, UREventManager::StaticClass());
	WayPointManager = NewObject<URWayPointManager>(this, URWayPointManager::StaticClass());
	
	if (IsValid(WayPointManager))
	{
		WayPointManager->Initialize();
	}
}
