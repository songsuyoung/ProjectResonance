#include "NPC/Task/Farm/RSTTask_Plow.h"

#include "GameFramework/Character.h"

EStateTreeRunStatus URSTTask_Plow::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	OwnerCharacter->PlayAnimMontage(PlowAnimation);
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus URSTTask_Plow::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	if (ElapsedTime <= WorkDuration)
	{
		// 상하좌우 중 하나 선택해서 이동 
		// 다시 밭질 
		WorkDuration = 0.0f;
		return EStateTreeRunStatus::Succeeded;
	}
	
	WorkDuration += DeltaTime;
	
	return EStateTreeRunStatus::Running;
}
