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

TArray<FName> URRegionManager::GetNextRegions(FName RegionID, EREmotionState NPCEmotionState)
{
	TArray<FName> CandidateRegions;
	Regions.GenerateKeyArray(CandidateRegions);
	
	CandidateRegions.Remove(RegionID); //RegionID 제거
	
	int32 RandomCount = FMath::RandRange(1, 3); //1~3개 포함해서 전달.

	if (CandidateRegions.Num() < RandomCount)
	{
		return TArray<FName>();
	}
	
	// 뒤에서 부터 현재 Index랑 교체할 Index값 뽑아서 셔플 진행
	for (int32 Index = CandidateRegions.Num() - 1; Index >= 0; --Index)
	{
		int32 SelectedIndex = FMath::RandRange(0, Index);
		CandidateRegions.Swap(Index, SelectedIndex);
	}
	
	CandidateRegions.SetNum(RandomCount);
	
	return CandidateRegions;
}

TWeakObjectPtr<ARRegionVolume> URRegionManager::GetRegionVolume(FName RegionID)
{
	if (Regions.Contains(RegionID))
	{
		 return Regions[RegionID];
	}
	
	return nullptr;
}
