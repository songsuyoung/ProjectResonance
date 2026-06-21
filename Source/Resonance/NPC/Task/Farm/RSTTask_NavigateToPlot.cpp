#include "NPC/Task/Farm/RSTTask_NavigateToPlot.h"

#include "AIController.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"

#include "Actor/RFarmPlotVolume.h"
#include "Animation/RAnimInstance.h"

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
	if (false == FarmVolume->FindNearestPlotRequiringTask(PendingFarmTask, OwnerLocation, PlotData))
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
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus URSTTask_NavigateToPlot::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	if (PathIndex >= PathPoints.Num())
	{
		PathIndex = 0;
		return EStateTreeRunStatus::Succeeded;
	}
	FVector CurrentLocation = OwnerCharacter->GetActorLocation();
	FVector Direction = (PathPoints[PathIndex] - CurrentLocation);
	Direction.Normalize();
	
	// 회전
	FRotator TargetRotation = Direction.Rotation();
	TargetRotation.Roll = TargetRotation.Pitch = 0.f;
	OwnerCharacter->SetActorRotation(FMath::RInterpTo(OwnerCharacter->GetActorRotation(), TargetRotation, DeltaTime, 10.f));
	// 블렌드스페이스는 로컬 스페이스 기준이므로 월드 방향을 로컬로 변환
	AnimInstance->SetDesiredDirection(Direction, ERAnimType::RootMotion);
		
	if (FVector::Dist2D(CurrentLocation, PathPoints[PathIndex]) <= 50.f)
	{
		PathIndex++;
	}
	
	return EStateTreeRunStatus::Running;
}