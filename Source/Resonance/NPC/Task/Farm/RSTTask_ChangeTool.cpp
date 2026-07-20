#include "NPC/Task/Farm/RSTTask_ChangeTool.h"

#include "Character/RNPCCharacter.h"
#include "Components/RJobComponent.h"

EStateTreeRunStatus URSTTask_ChangeTool::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	if (false == JobComponent.IsValid())
	{
		JobComponent = OwnerCharacter->GetJobComponent();
	}

	if (false == bClear)
	{
		JobComponent->ApplyToolMesh(ToolType);
	}
	else
	{
		JobComponent->ClearToolMesh();
	}
	
	return EStateTreeRunStatus::Succeeded;
}
