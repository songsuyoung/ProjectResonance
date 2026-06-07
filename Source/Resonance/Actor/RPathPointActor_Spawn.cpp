#include "Actor/RPathPointActor_Spawn.h"

#include "System/RNPCSpawnManager.h"

ARPathPointActor_Spawn::ARPathPointActor_Spawn()
	: Super()
{

}

void ARPathPointActor_Spawn::BeginPlay()
{
	Super::BeginPlay();

	URNPCSpawnManager* SpawnManager = URNPCSpawnManager::Get(this);
	
	check(SpawnManager);
	
	SpawnManager->SpawnNPC();
}
