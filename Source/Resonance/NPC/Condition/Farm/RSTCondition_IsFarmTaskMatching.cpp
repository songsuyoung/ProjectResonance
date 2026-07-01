#include "RSTCondition_IsFarmTaskMatching.h"

#include "Actor/RFarmPlotVolume.h"

bool URSTCondition_IsFarmTaskMatching::TestCondition(FStateTreeExecutionContext& Context) const
{
	if (false == FarmVolume.IsValid())
	{
		return false;
	}
	
	FRPlotData PlotData;
	if (false == FarmVolume->GetActivePlot(PlotData))
	{
		return false;
	}
	
	if (PlotData.PendingTasks.IsEmpty())
	{
		return false;
	}
	
	return ExpectedFarmTasks.Contains(PlotData.PendingTasks[0]);
}
