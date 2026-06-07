#include "System/RGameInstance.h"

// Newly Created File Names ..
#include "System/RPathFinder.h"
#include "RBakeDataManager.h"
#include "RNPCSpawnManager.h"
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
}
