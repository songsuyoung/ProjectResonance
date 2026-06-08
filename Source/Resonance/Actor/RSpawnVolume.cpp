#include "Actor/RSpawnVolume.h"

#include "System/RNPCSpawnManager.h"

void ARSpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
	URNPCSpawnManager* NPCSpawnManager = URNPCSpawnManager::Get(this);
	
	check(NPCSpawnManager);
	
	// 현재 태그에 맞는 장소에, NPC 스폰해줘를 의미.
	NPCSpawnManager->SpawnNPC(RegionID, GetActorTransform());
}
