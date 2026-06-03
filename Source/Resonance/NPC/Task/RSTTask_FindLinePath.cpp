#include "RSTTask_FindLinePath.h"

// UE5
#include "StateTreeExecutionContext.h"
#include "GameplayTagContainer.h"

// 
#include "Data/ResonanceStructs.h"
#include "System/RPathFinder.h"

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
	int32 DestinationPointIndex = PathFinder->PickDestination();
	
	TArray<FVector> Locations = PathFinder->FindPath(NearestPointIndex, DestinationPointIndex);
	// 데이터 전달용으로 시작점과 끝점의 위치를 담는다.
	FRPathRoutePayload Payload({Locations});
	
	// SendEvent로 어떤 위치가 결정됐는지 전달한다.
	Context.SendEvent(FGameplayTag::RequestGameplayTag(TEXT("StateTree.Task.Event.DestinationDecided")), FConstStructView::Make(Payload));
	
	return Super::EnterState(Context, Transition);
}
