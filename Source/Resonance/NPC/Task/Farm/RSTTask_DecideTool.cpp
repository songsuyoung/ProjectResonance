#include "NPC/Task/Farm/RSTTask_DecideTool.h"

#include "Actor/RFarmPlotVolume.h"
#include "Character/RNPCCharacter.h"
#include "Components/RJobComponent.h"
#include "Resonance/Data/ResonanceEnums.h"

static ERToolType GetToolTypeForFarmTask(ERequiredFarmTask FarmTask)
{
	switch (FarmTask)
	{
		case ERequiredFarmTask::Till:  return ERToolType::Hoe;
		case ERequiredFarmTask::Sow:   return ERToolType::None;
		case ERequiredFarmTask::Clear: return ERToolType::Shovel;
		case ERequiredFarmTask::Water: return ERToolType::WaterPail;
		default:                       return ERToolType::None;
	}
}

EStateTreeRunStatus URSTTask_DecideTool::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	if (false == OwnerCharacter.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}

	if (false == FarmVolume.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}

	FRPlotData PlotData;
	if (false == FarmVolume->GetActivePlot(PlotData))
	{
		return EStateTreeRunStatus::Failed;
	}

	URJobComponent* JobComponent = OwnerCharacter->GetJobComponent();
	if (false == IsValid(JobComponent))
	{
		return EStateTreeRunStatus::Failed;
	}

	ERToolType RequiredToolType = GetToolTypeForFarmTask(PlotData.RequiredFarmTask);
	JobComponent->SetPendingToolType(RequiredToolType);

	return EStateTreeRunStatus::Succeeded;
}
