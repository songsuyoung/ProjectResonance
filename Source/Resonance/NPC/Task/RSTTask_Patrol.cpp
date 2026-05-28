#include "NPC/Task/RSTTask_Patrol.h"

// UE 5.
#include "EngineUtils.h"
#include "AIController.h"
#include "Components/SplineComponent.h"

//
#include "VisualizeTexture.h"
#include "Actor/RPatrolActor.h"
#include "Character/RBaseCharacter.h"


EStateTreeRunStatus URSTTask_Patrol::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	if (false == AIController.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}
	
	EStateTreeRunStatus PtrRunStatus = Super::EnterState(Context, Transition);
	
	UWorld* World = GetWorld();
	
	check(World);
	
	ARPatrolActor* PatrolActor = nullptr;
	
	for (TActorIterator<ARPatrolActor> It(World); It; ++It)
	{
		PatrolActor = *It;
	}
	
	if (false == IsValid(PatrolActor))
	{
		return EStateTreeRunStatus::Failed;
	}
	
	AIController->ReceiveMoveCompleted.AddDynamic(this, &ThisClass::ReceiveMoveCompleted);
	SplineComponent = PatrolActor->GetSplineComponent();
	
	if (false == SplineComponent.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}
	SplinePointIndex = 0;
	LastSplinePointIndex = SplineComponent->GetNumberOfSplinePoints();
	
	const FSplinePoint& SplinePoint = SplineComponent->GetSplinePointAt(SplinePointIndex, ESplineCoordinateSpace::World);
	AIController->MoveToLocation(SplinePoint.Position, 20.f);
	
	return EStateTreeRunStatus::Running;
}

void URSTTask_Patrol::ReceiveMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	SplinePointIndex ++;
	
	if (SplinePointIndex >= LastSplinePointIndex)
	{
		FinishTask(true);
		return;
	}
	
	const FSplinePoint& SplinePoint = SplineComponent->GetSplinePointAt(SplinePointIndex, ESplineCoordinateSpace::World);
	AIController->MoveToLocation(SplinePoint.Position, 20.f);
}
