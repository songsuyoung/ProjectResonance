#include "System/RGameInstance.h"

// Newly Created File Names ..
#include "System/RPathFinder.h"
#include "RBakeDataManager.h"
#include "REventManager.h"
#include "RNPCSpawnManager.h"
#include "RRegionManager.h"
#include "RTimeManager.h"
#include "RUIManagerBase.h"
#include "System/RDataManager.h"

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
	BakeDataManager = NewObject<URBakeDataManager>(this, URBakeDataManager::StaticClass());
	PathFinder = NewObject<URPathFinder>(this, URPathFinder::StaticClass());

	if (IsValid(BakeDataManager))
	{
		BakeDataManager->Initialize();
	}
	
	if (IsValid(PathFinder))
	{
		PathFinder->Initialize();
	}
	
	NPCSpawnManager = NewObject<URNPCSpawnManager>(this, NPCSpawnManagerClass);
	if (IsValid(NPCSpawnManager))
	{
		NPCSpawnManager->Initialize();
	}
	RegionManager = NewObject<URRegionManager>(this, URRegionManager::StaticClass());
	
	UIManager = NewObject<URUIManagerBase>(this, UIManagerClass);
}

void URGameInstance::BeginDestroy()
{
	Super::BeginDestroy();
	
	if (IsValid(NPCSpawnManager))
	{
		NPCSpawnManager->DeInitialize();
	}
}
