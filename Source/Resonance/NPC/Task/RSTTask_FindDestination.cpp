#include "RSTTask_FindDestination.h"

// UE
#include "StateTreeExecutionContext.h"
#include "GameplayTagContainer.h"

// 
#include "Data/ResonanceStructs.h"
#include "System/RPathFinder.h"

EStateTreeRunStatus URSTTask_FindDestination::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	URPathFinder* PathFinder = URPathFinder::Get(this);
	check(PathFinder);
	
	// 시작점과 끝점을 랜덤으로 정한다.
	int32 NearestPointIndex = PathFinder->GetNearestNodeIndex(OwnerCharacter->GetActorLocation());
	int32 DestinationPointIndex = PathFinder->PickDestination();
	
	// 데이터 전달용으로 시작점과 끝점의 위치를 담는다.
	FRPathRoutePayload Payload({NearestPointIndex, DestinationPointIndex});
	
	// SendEvent로 어떤 위치가 결정됐는지 전달한다.
	Context.SendEvent(FGameplayTag::RequestGameplayTag(TEXT("StateTree.Task.Event.DestinationDecided")), FConstStructView::Make(Payload));
	
	return Super::EnterState(Context, Transition);
}
