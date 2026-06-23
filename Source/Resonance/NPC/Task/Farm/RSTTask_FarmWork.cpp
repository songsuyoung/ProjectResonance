#include "NPC/Task/Farm/RSTTask_FarmWork.h"

#include "Actor/RFarmPlotVolume.h"
#include "GameFramework/Character.h"

EStateTreeRunStatus URSTTask_FarmWork::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	if (false == OwnerCharacter.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}
	
	if (false == FarmVolume.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}
	
	// PendingTask인 나랑 가장 가까운 위치를 찾는다.
	FRPlotData PlotData;
	if (false == FarmVolume->GetActivePlot(PlotData))
	{
		return EStateTreeRunStatus::Failed;
	}
	
	ElapsedTime = 0.f;
	OwnerCharacter->PlayAnimMontage(Animation);
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus URSTTask_FarmWork::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	ElapsedTime += DeltaTime;
	if (ElapsedTime >= WorkDuration)
	{
		// 상하좌우 중 하나 선택해서 이동 
		// 다시 밭질 
		FarmVolume->CompleteCurrentPlot(FarmTask);
		OwnerCharacter->StopAnimMontage(Animation);
		return EStateTreeRunStatus::Succeeded;
	}
	
	return EStateTreeRunStatus::Running;
}
