#include "NPC/Condition/RSTCondition_IsExhausted.h"

// 
#include "Character/RBaseCharacter.h"
#include "Components/Stat/RBaseStatComponent.h"

bool URSTCondition_IsExhausted::TestCondition(FStateTreeExecutionContext& Context) const
{
	if (false == OwnerCharacter.IsValid())
	{
		return false;
	}
	
	URBaseStatComponent* BaseStatComponent = OwnerCharacter->GetBaseStatComponent();
	
	if (false == IsValid(BaseStatComponent))
	{
		return false;
	}
	
	return BaseStatComponent->GetCurrentStatValue(ERStatType::Stamina) <= 0.f;
}
