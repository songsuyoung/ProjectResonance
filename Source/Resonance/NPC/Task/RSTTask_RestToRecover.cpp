#include "RSTTask_RestToRecover.h"

#include "Character/RBaseCharacter.h"
#include "Components/Stat/RBaseStatComponent.h"

URSTTask_RestToRecover::URSTTask_RestToRecover(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, TargetStamaina(0.f)
	, TargetPercentage(0.2f)
{
}

EStateTreeRunStatus URSTTask_RestToRecover::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	if (false == OwnerCharacter.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}
	
	StatComponent = OwnerCharacter->GetBaseStatComponent();
	
	if (false == StatComponent.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}
	
	TargetStamaina = StatComponent->GetMaxStatValue(ERStatType::Stamina) * TargetPercentage;
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus URSTTask_RestToRecover::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	float CurrentStamina = StatComponent->GetCurrentStatValue(ERStatType::Stamina);
	
	if (CurrentStamina > TargetStamaina)
	{
		return EStateTreeRunStatus::Succeeded;
	}
	
	CurrentStamina = CurrentStamina + RecoveryRate * DeltaTime;
	StatComponent->UpdateStat(ERStatType::Stamina, CurrentStamina);
		
	return EStateTreeRunStatus::Running;
}
