#include "NPC/Condition/RSTCondition_IsWorkTime.h"

// 
#include "Character/RBaseCharacter.h"
#include "System/RTimeManager.h"

bool URSTCondition_IsWorkTime::TestCondition(FStateTreeExecutionContext& Context) const
{
	if (false == OwnerCharacter.IsValid())
	{
		return false;
	}
	
	URTimeManager* TimeManager = URTimeManager::Get(OwnerCharacter.Get());
	int32 CurrentHour = TimeManager->GetHourTime();
	
	bool bIsWorkTime = CurrentHour >= StartHour && CurrentHour <= EndHour;
	return bInvert? !bIsWorkTime : bIsWorkTime;
}
