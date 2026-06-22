#include "RSTCondition_IsFarmTaskMatching.h"

#include "Actor/RFarmPlotVolume.h"

bool URSTCondition_IsFarmTaskMatching::TestCondition(FStateTreeExecutionContext& Context) const
{
	if (false == FarmVolume.IsValid())
	{
		return false;
	}
	
	ERequiredFarmTask PendingFarmTask = FarmVolume->GetPendingFarmTask();
	
	return ExpectedFarmTasks.Contains(PendingFarmTask);
}
