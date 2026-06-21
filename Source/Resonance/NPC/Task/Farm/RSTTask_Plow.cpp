#include "NPC/Task/Farm/RSTTask_Plow.h"

#include "Actor/RFarmPlotVolume.h"
#include "GameFramework/Character.h"

EStateTreeRunStatus URSTTask_Plow::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	if (false == OwnerCharacter.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}
	
	if (false == FarmVolume.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}
	
	FVector OwnerLocation = OwnerCharacter->GetActorLocation();
	// PendingTask인 나랑 가장 가까운 위치를 찾는다.
	FRPlotData PlotData;
	if (false == FarmVolume->GetCurrentPlot(PlotData))
	{
		return EStateTreeRunStatus::Failed;
	}
	
	WorkDuration = 0.0f;
	WorkIndex = PlotData.Index;
	OwnerCharacter->PlayAnimMontage(PlowAnimation);
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus URSTTask_Plow::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	if (ElapsedTime <= WorkDuration)
	{
		// 상하좌우 중 하나 선택해서 이동 
		// 다시 밭질 
		FarmVolume->MarkPlotVisited(FarmTask, WorkIndex);
		OwnerCharacter->StopAnimMontage(PlowAnimation);
		return EStateTreeRunStatus::Succeeded;
	}
	
	WorkDuration += DeltaTime;
	
	return EStateTreeRunStatus::Running;
}
