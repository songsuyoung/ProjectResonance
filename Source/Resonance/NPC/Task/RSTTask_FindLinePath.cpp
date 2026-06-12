#include "RSTTask_FindLinePath.h"

// UE5
#include "StateTreeExecutionContext.h"
#include "GameplayTagContainer.h"

// 
#include "Character/RNPCCharacter.h"
#include "Data/ResonanceStructs.h"
#include "System/RPathFinder.h"
#include "System/RRegionManager.h"

URSTTask_FindLinePath::URSTTask_FindLinePath(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

EStateTreeRunStatus URSTTask_FindLinePath::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	URPathFinder* PathFinder = URPathFinder::Get(this);
	check(PathFinder);
	
	// 시작점과 끝점을 랜덤으로 정한다.
	int32 NearestPointIndex = PathFinder->GetNearestNodeIndex(OwnerCharacter->GetActorLocation());
	
	// 이 RegionID는 다음 목표지점으로 정하면 안된다!!!!!
	FName RegionID = OwnerCharacter->PeekNextRegion();
	
 	URRegionManager* RegionManager = URRegionManager::Get(this);
	
	check(RegionManager);
	
	FVector Location;
	bool bFind = RegionManager->FindRegionLocation(RegionID, Location);
	
	if (false == bFind)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	int32 DestinationPointIndex = PathFinder->GetNearestNodeIndex(Location);
	if (NearestPointIndex ==  INDEX_NONE || DestinationPointIndex ==  INDEX_NONE)
	{
		return EStateTreeRunStatus::Failed;
	}	
	
	TArray<FVector> Locations = PathFinder->FindPath(NearestPointIndex, DestinationPointIndex);
	// 데이터 전달용으로 시작점과 끝점의 위치를 담는다.
	FRPathRoutePayload Payload({RegionID, Locations});
	
	// SendEvent로 어떤 위치가 결정됐는지 전달한다.
	Context.SendEvent(FGameplayTag::RequestGameplayTag(TEXT("StateTree.Task.Event.DestinationDecided")), FConstStructView::Make(Payload));
	
	return Super::EnterState(Context, Transition);
}
