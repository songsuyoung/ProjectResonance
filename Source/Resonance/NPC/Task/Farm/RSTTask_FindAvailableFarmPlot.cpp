#include "NPC/Task/Farm/RSTTask_FindAvailableFarmPlot.h"

#include "Actor/RFarmPlotVolume.h"

EStateTreeRunStatus URSTTask_FindAvailableFarmPlot::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	if (false == FarmVolume.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}
	
	if (false == FarmVolume->UpdateNextFarmWideTask())
	{
		return EStateTreeRunStatus::Failed;
	}
	
	return EStateTreeRunStatus::Succeeded;
}
