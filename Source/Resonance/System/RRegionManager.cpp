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

	TMap<FName, float> WeightedRegions;
	// 선호 지역과 비선호 지역에 대해 가중치를 적용
	for (const FName& CandidateRegion : CandidateRegions)
	{
		float Weight = 1.0f;
		
		const float* PreferredWeight = PreferredRegions.Find(CandidateRegion);
		const float* DislikedWeight = DislikedRegions.Find(CandidateRegion);
		
		if (PreferredWeight != nullptr)
		{
			Weight *= *PreferredWeight;
		}
		else if (DislikedWeight != nullptr)
		{
			Weight *= *DislikedWeight;
		}
		else
		{
			// 나머지 일반적인 경우
			Weight *= 0.5f;
		}
		
		WeightedRegions.Add(CandidateRegion, Weight);
	}
	
	
	float* CurrentRegionWeight = WeightedRegions.Find(RegionID);
	
	if (nullptr == CurrentRegionWeight)
	{
		return TArray<FName>();
	}
	
	//의도적으로 낮춤. 0.5*0.5 => 0.25이 된다.
	*CurrentRegionWeight *= 0.5f;
	//누적 확률 추첨
	float TotalWeight = 0.f;
	for (const auto& WeightedRegion : WeightedRegions)
	{
		TotalWeight += WeightedRegion.Value;
	}
	
	int32 Count = FMath::RandRange(1, 3);
	
	TSet<FName> NextRegions;
	for (int Index = 0; Index < Count; Index++)
	{
		// 사이에서 랜덤 값 고르기
		float RandomValue = FMath::FRandRange(0.f, TotalWeight);
		float Accumulated = 0.f; //누적 해서 초과하면 선택 예정
		
		for (const auto& WeightedRegion : WeightedRegions)
		{
			Accumulated += WeightedRegion.Value;
		
			if (Accumulated >= RandomValue)
			{
				// 어차피 TSet이여서 중복안되니까. Add로 해도된다.
				NextRegions.Add(WeightedRegion.Key);
				break;
			}
		}
		
	}
	
	return NextRegions.Array();
}

TWeakObjectPtr<ARRegionVolume> URRegionManager::GetRegionVolume(FName RegionID)
{
	if (Regions.Contains(RegionID))
	{
		 return Regions[RegionID];
	}
	
	return nullptr;
}
