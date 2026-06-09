#include "System/RNPCSpawnManager.h"

// 
#include "RGameInstance.h"
#include "RRegionManager.h"
#include "Character/RNPCCharacter.h"

URNPCSpawnManager* URNPCSpawnManager::Get(UObject* Context)
{
	UWorld* World = Context->GetWorld();

	check(World);

	URGameInstance* GameInstance = Cast<URGameInstance>(World->GetGameInstance());

	check(GameInstance);

	return GameInstance->GetNPCSpawnManager();
}

void URNPCSpawnManager::SpawnNPC(const FName& RegionID)
{
	URRegionManager* RegionManager = URRegionManager::Get(this);
	check(RegionManager);
	
	//RegionDataTable 에 NPC ID들이 저장, NPC를 스폰하는데 사용하도록 함.
	FVector Location;
	bool bFind = RegionManager->FindRegionLocation(RegionID, Location);
	
	if (false == bFind)
	{
		return;
	}
	
	UWorld* World = GetWorld();
	check(World);
	
	FActorSpawnParameters SpawnParams;
	ActiveNPC.Add(World->SpawnActor<ARNPCCharacter>(NPCCharacterClass, Location, FRotator::ZeroRotator, SpawnParams));
}
