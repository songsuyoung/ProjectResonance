#include "NPC/Task/Farm/RSTTask_FarmWork.h"

#include "Actor/RFarmPlotVolume.h"
#include "Character/RNPCCharacter.h"
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
	
	if (false == IsValid(WorkMontage))
	{
		return EStateTreeRunStatus::Failed;
	}
	
	FRPlotData PlotData;
	// PendingTask인 나랑 가장 가까운 위치를 찾는다.
	if (false == FarmVolume->GetActivePlot(PlotData))
	{
		return EStateTreeRunStatus::Failed;
	}
	ActivePlotIndex = PlotData.Index;

	int32 Count = FMath::RandRange(2, 5);
	WorkDuration = WorkMontage->GetPlayLength() * Count;
	ElapsedTime = 0.f;
	OwnerCharacter->PlayAnimMontage(WorkMontage);
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus URSTTask_FarmWork::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	ElapsedTime += DeltaTime;
	if (ElapsedTime >= WorkDuration)
	{
		// 상하좌우 중 하나 선택해서 이동 
		// 다시 밭질 
		FarmVolume->CompleteCurrentPlot(ActivePlotIndex);
		OwnerCharacter->StopAnimMontage(WorkMontage);
		return EStateTreeRunStatus::Succeeded;
	}
	
	return EStateTreeRunStatus::Running;
}