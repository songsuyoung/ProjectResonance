#include "RSTTask_FindDestination.h"

// UE
#include "EngineUtils.h"
#include "StateTreeExecutionContext.h"
#include "GameplayTagContainer.h"

// 
#include "Actor/RPatrolActor.h"
#include "Data/ResonanceStructs.h"

EStateTreeRunStatus URSTTask_FindDestination::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	TArray<AActor*> PatrolPoints;
	// 배치된 PatrolActor 을 모두 가져온다.
	for (TActorIterator<ARPatrolActor> It(GetWorld()); It; ++It)
	{
		ARPatrolActor* Actor = *It;
		
		if (IsValid(Actor))
		{
			PatrolPoints.Add(Actor);
		}
	}
	
	// 시작점과 끝점을 랜덤으로 정한다.
	int32 DestinationPointIndex = FMath::RandRange(0, PatrolPoints.Num() - 1);
	
	// 데이터 전달용으로 시작점과 끝점의 위치를 담는다.
	FRPatrolRoutePayload Payload(PatrolPoints[DestinationPointIndex]->GetActorLocation());
	
	// SendEvent로 어떤 위치가 결정됐는지 전달한다.
	Context.SendEvent(FGameplayTag::RequestGameplayTag(TEXT("StateTree.Task.Event.DestinationDecided")), FConstStructView::Make(Payload));
	
	return Super::EnterState(Context, Transition);
}
