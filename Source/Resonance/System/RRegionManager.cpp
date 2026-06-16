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

TArray<FName> URRegionManager::GetNextRegions(FName RegionID, const TMap<FName, float>& PreferredRegions, const TMap<FName, float>& DislikedRegions)
{
    TArray<FName> CandidateRegions;
    Regions.GenerateKeyArray(CandidateRegions);

    // 현재 지역 제외
    CandidateRegions.Remove(RegionID);

    // 가중치 계산
    TArray<TPair<FName, float>> WeightedRegions;
    float TotalWeight = 0.f;

    for (const FName& Region : CandidateRegions)
    {
        float Weight = 0.005f; // 기본 0.5%

        if (const float* PreferredWeight = PreferredRegions.Find(Region))
        {
            int32 Rank = 0;
            for (const auto& Pair : PreferredRegions)
            {
                Rank++;
                if (Pair.Key == Region) break;
            }

            switch (Rank)
            {
                case 1: Weight = 0.008f; break;
                case 2: Weight = 0.007f; break;
                case 3: Weight = 0.006f; break;
                default: Weight = 0.005f; break;
            }

            Weight += *PreferredWeight;
        }
        else if (const float* DislikedWeight = DislikedRegions.Find(Region))
        {
            int32 Rank = 0;
            for (const auto& Pair : DislikedRegions)
            {
                Rank++;
                if (Pair.Key == Region) break;
            }

            switch (Rank)
            {
                case 1: Weight = 0.002f; break;
                case 2: Weight = 0.003f; break;
                case 3: Weight = 0.004f; break;
                default: Weight = 0.005f; break;
            }

            Weight -= *DislikedWeight;
            Weight = FMath::Max(Weight, 0.0f);
        }

        WeightedRegions.Add({ Region, Weight });
        TotalWeight += Weight;
    }

    TArray<FName> SortedRegions;

    // 전체 가중치 0 방어
    if (TotalWeight <= 0.f)
    {
        return SortedRegions;
    }

    // 이미 선택된 지역 추적
    TSet<FName> SelectedRegions;

    while (SortedRegions.Num() < WeightedRegions.Num())
    {
        float RandomValue = FMath::FRandRange(0.f, TotalWeight);
        float Cumulative = 0.f;

        for (const auto& Pair : WeightedRegions)
        {
            // 이미 선택된 지역은 스킵
            if (SelectedRegions.Contains(Pair.Key)) continue;

            Cumulative += Pair.Value;
            if (RandomValue <= Cumulative)
            {
                SortedRegions.Add(Pair.Key);
                SelectedRegions.Add(Pair.Key);
                TotalWeight -= Pair.Value; // 선택된 가중치 제거
                break;
            }
        }
    }

    return SortedRegions;
}

TWeakObjectPtr<ARRegionVolume> URRegionManager::GetRegionVolume(FName RegionID)
{
	if (Regions.Contains(RegionID))
	{
		 return Regions[RegionID];
	}
	
	return nullptr;
}
