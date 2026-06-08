#include "System/RNPCSpawnManager.h"

#include "RDataManager.h"
#include "RGameInstance.h"
#include "RPathFinder.h"
#include "Character/RNPCCharacter.h"
#include "Data/RCoreEnums.h"
#include "Data/RRegionDataTable.h"


URNPCSpawnManager* URNPCSpawnManager::Get(UObject* Context)
{
	UWorld* World = Context->GetWorld();

	check(World);

	URGameInstance* GameInstance = Cast<URGameInstance>(World->GetGameInstance());

	check(GameInstance);

	return GameInstance->GetNPCSpawnManager();
}

void URNPCSpawnManager::SpawnNPC(const FName& RegionID, const FTransform& SpawnTransform)
{
	URDataManager* DataManager = URDataManager::Get(this);
	
	check(DataManager);
	
	FRRegionDataTable* RegionDataTable = DataManager->GetDataTableRow<FRRegionDataTable>(ERDataTableType::RegionData, RegionID);

	if (nullptr == RegionDataTable)
	{
		return;
	}
	
	//RegionDataTable 에 NPC ID들이 저장, NPC를 스폰하는데 사용하도록 함.
	const FVector& Location = SpawnTransform.GetLocation();
	
	UWorld* World = GetWorld();
	check(World);
	
	FActorSpawnParameters SpawnParams;
	ActiveNPC.Add(World->SpawnActor<ARNPCCharacter>(NPCCharacterClass, Location, FRotator::ZeroRotator, SpawnParams));
}
