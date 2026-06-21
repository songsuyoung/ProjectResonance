#include "RSTCondition_IsFarmTaskMatching.h"

bool URSTCondition_IsFarmTaskMatching::TestCondition(FStateTreeExecutionContext& Context) const
{
	return ExpectedFarmTasks.Contains(PendingFarmTask);
}
