#include "NPC/Condition/Farm/RSTCondition_IsToolChangeRequired.h"

#include "Character/RNPCCharacter.h"
#include "Components/RJobComponent.h"

bool URSTCondition_IsToolChangeRequired::TestCondition(FStateTreeExecutionContext& Context) const
{
	if (false == OwnerCharacter.IsValid())
	{
		return false;
	}

	URJobComponent* JobComponent = OwnerCharacter->GetJobComponent();
	if (false == IsValid(JobComponent))
	{
		return false;
	}

	return JobComponent->IsToolChangeRequired();
}
