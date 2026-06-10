#include "System/RRegionManager.h"

#include "RGameInstance.h"
#include "RNPCSpawnManager.h"
#include "Actor/RRegionVolume.h"


URRegionManager* URRegionManager::Get(UObject* Context)
{
	UWorld* World = Context->GetWorld();

	check(World);

	URGameInstance* GameInstance = Cast<URGameInstance>(World->GetGameInstance());

	check(GameInstance);

	return GameInstance->GetRegionManager();
}

void URRegionManager::RegisterRegion(FName ID, ARRegionVolume* RegionVolume)
{
	Regions.Add(ID, RegionVolume);
}

bool URRegionManager::FindRegionLocation(FName ID, FVector& OutLocation)
{
	if (false == Regions.Contains(ID) || false == Regions[ID].IsValid())
	{
		return false;
	}
	
	OutLocation = Regions[ID]->GetActorLocation();
	return true;
}