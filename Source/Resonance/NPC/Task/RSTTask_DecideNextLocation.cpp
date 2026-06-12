#include "RSTTask_DecideNextLocation.h"

// UE
#include "StateTreeExecutionContext.h"

// 
#include "Character/RNPCCharacter.h"
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

	// NPC Character에게 현재 있는 위치가 어디있는지 가져온다.	
	FName RegionID = OwnerCharacter->GetCurrentVisitedRegion();
	
	// TODO: 추후 NPC 감정 상태 연동 예정
	EREmotionState EmotionState = EREmotionState::Calm;
	
	// 그 위치를 제외한 다른 위치 선택해달라고 RegionManager에게 요청한다.
	URRegionManager* RegionManager = URRegionManager::Get(this);
	
	check(RegionManager);
	
	const TArray<FName>& NextRegions = RegionManager->GetNextRegions(RegionID, EmotionState);
	
	if (NextRegions.IsEmpty())
	{
		return EStateTreeRunStatus::Failed;
	}
	// NextRegion이 없으면 채운다.
	// NextRegion을 채운다. (랜덤으로 여러개 채울지, 1개만 채울지를 결정) => 1~3개 사이로 결정.
	OwnerCharacter->SetNextRegions(NextRegions);
	
	return EStateTreeRunStatus::Succeeded;
}
