#include "RSTTask_DecideNextLocation.h"

// UE
#include "StateTreeExecutionContext.h"

// 
#include "Character/RNPCCharacter.h"
#include "Components/Emotion/REmotionComponent.h"
#include "Components/Stat/RBaseStatComponent.h"
#include "Data/RCommonEnums.h"
#include "Data/ResonanceEnums.h"
#include "System/RRegionManager.h"

URSTTask_DecideNextLocation::URSTTask_DecideNextLocation(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

EStateTreeRunStatus URSTTask_DecideNextLocation::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	// 이미 다음 구역이 있기 때문에 결정하지 않아도 된다.
	if (OwnerCharacter->HasNextRegion())
	{
		return EStateTreeRunStatus::Succeeded;
	}

	// 가야할 곳이 이미 정해진 경우가 있음.
	if (NextRegionID.IsValid())
	{
		OwnerCharacter->SetNextRegions({NextRegionID});
		return EStateTreeRunStatus::Succeeded;
	}
	
	// NPC Character에게 현재 있는 위치가 어디있는지 가져온다.	
	FName RegionID = OwnerCharacter->GetCurrentVisitedRegion();
	
	URBaseStatComponent* BaseStatComponent = OwnerCharacter->GetBaseStatComponent();
	
	if (nullptr == BaseStatComponent)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	TMap<FName, float> PreferredRegions, DislikedRegions;
	BaseStatComponent->GetRegionPreferences(PreferredRegions, DislikedRegions);
	
	UREmotionComponent* EmotionComponent = OwnerCharacter->GetEmotionComponent();
	
	if (nullptr == EmotionComponent)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	// TODO: 추후 NPC 감정 상태 연동 예정
	EREmotionState EmotionState = EmotionComponent->GetEmotionState();
	// 여기서 감정에 대해 PreferredRegions, DislikedRegion 확률 계산
	// 감정이 안좋을 수록, Alpha값이 추가되어진다.
	// 0~3까지는 Alpha 거꾸로 Alpha +된다
	float Alpha = (1.f - ((float)EmotionState/ (float)EREmotionState::Max)) * 0.5f;
	
	for (auto& PreferredRegion : PreferredRegions)
	{
		PreferredRegion.Value += Alpha;
	}
	
	// 그 위치를 제외한 다른 위치 선택해달라고 RegionManager에게 요청한다.
	URRegionManager* RegionManager = URRegionManager::Get(this);
	
	check(RegionManager);
	
	const TArray<FName>& NextRegions = RegionManager->GetNextRegions(RegionID, PreferredRegions, DislikedRegions);
	
	if (NextRegions.IsEmpty())
	{
		return EStateTreeRunStatus::Failed;
	}
	// NextRegion이 없으면 채운다.
	// NextRegion을 채운다. (랜덤으로 여러개 채울지, 1개만 채울지를 결정) => 1~3개 사이로 결정.
	OwnerCharacter->SetNextRegions(NextRegions);
	
	return EStateTreeRunStatus::Succeeded;
}
