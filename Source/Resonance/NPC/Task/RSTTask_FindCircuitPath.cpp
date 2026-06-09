#include "RSTTask_FindCircuitPath.h"

// UE
#include "StateTreeExecutionContext.h"
#include "GameplayTagContainer.h"

// 
#include "Data/ResonanceStructs.h"
#include "System/RPathFinder.h"

URSTTask_FindCircuitPath::URSTTask_FindCircuitPath(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

EStateTreeRunStatus URSTTask_FindCircuitPath::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	/*
	URPathFinder* PathFinder = URPathFinder::Get(this);
	check(PathFinder);
	
	const TArray<FVector>& Locations = PathFinder->FindPath_Circuit(OwnerCharacter->GetActorLocation());
	
	// 데이터 전달용으로 시작점과 끝점의 위치를 담는다.
	FRPathRoutePayload Payload({Locations});
	
	// SendEvent로 어떤 위치가 결정됐는지 전달한다.
	Context.SendEvent(FGameplayTag::RequestGameplayTag(TEXT("StateTree.Task.Event.DestinationDecided")), FConstStructView::Make(Payload));
	*/
	
	return Super::EnterState(Context, Transition);
}
