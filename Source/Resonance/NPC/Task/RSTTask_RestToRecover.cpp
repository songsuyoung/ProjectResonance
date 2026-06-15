#include "RSTTask_RestToRecover.h"

#include "Character/RBaseCharacter.h"
#include "Components/Stat/RBaseStatComponent.h"

URSTTask_RestToRecover::URSTTask_RestToRecover(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, StaminaRecoverTimer(0.f)
	, TargetStamaina(0.f)
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
	
	TargetStamaina = StatComponent->GetMaxStatValue(ERStatType::Stamina) * 0.1f;
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus URSTTask_RestToRecover::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	if (StaminaRecoverInterval < StaminaRecoverTimer)
	{
		float CurrentStamina = StatComponent->GetCurrentStatValue(ERStatType::Stamina);
		
		if (TargetStamaina < CurrentStamina)
		{
			return EStateTreeRunStatus::Succeeded;
		}
		
		// TODO: 공식 성립하기
		float NewStamina = CurrentStamina + 15.f;
		StatComponent->UpdateStat(ERStatType::Stamina, NewStamina);
		
		StaminaRecoverTimer = 0.f;
	}
	
	StaminaRecoverTimer += DeltaTime;
	
	return EStateTreeRunStatus::Running;
}
