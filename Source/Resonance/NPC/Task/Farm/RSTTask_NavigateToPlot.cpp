#include "NPC/Task/Farm/RSTTask_NavigateToPlot.h"

#include "AIController.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "StateTreeExecutionContext.h"
#include "GameFramework/Character.h"

#include "Actor/RFarmPlotVolume.h"
#include "Animation/RAnimInstance.h"
#include "Data/ResonanceStructs.h"

EStateTreeRunStatus URSTTask_NavigateToPlot::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	if (false == FarmVolume.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}
	
	if (false == AIController.IsValid())
	{
		return EStateTreeRunStatus::Failed;		
	}
	
	if (false == OwnerCharacter.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}
	
	AnimInstance = Cast<URAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	
	if (false == AnimInstance.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}
	FVector OwnerLocation = OwnerCharacter->GetActorLocation();
	// PendingTask인 나랑 가장 가까운 위치를 찾는다.
	FRPlotData PlotData;
	if (false == FarmVolume->FindNearestPendingPlot(OwnerLocation, PlotData))
	{
		return EStateTreeRunStatus::Failed;
	}
	
	UWorld* World = GetWorld();
	check(World);
	
	UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(World, OwnerLocation, PlotData.Location); 
	
	if (nullptr == NavigationPath)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	PathPoints= NavigationPath->PathPoints;
	PathIndex = 0;
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus URSTTask_NavigateToPlot::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	if (PathIndex >= PathPoints.Num())
	{
		return EStateTreeRunStatus::Succeeded;
	}
	
	FVector CurrentLocation = OwnerCharacter->GetActorLocation();
	FVector Direction = (PathPoints[PathIndex] - CurrentLocation);
	Direction.Normalize();
	
	AnimInstance->SetDesiredDirection(Direction, ERAnimType::RootMotion);

	if (FVector::Dist2D(CurrentLocation, PathPoints[PathIndex]) <= 50.f)
	{
		PathIndex++;
	}
	
	return EStateTreeRunStatus::Running;
}