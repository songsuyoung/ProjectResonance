#include "StateContext/RStateContext.h"

#include "Components/RActionStateComponent.h"

void URStateContext::Initialize(URActionStateComponent* InActionStateComponent)
{
	ActionStateComponent = InActionStateComponent;
}

// ActionStateComponent 로부터 현재 가지고 있는 모든 StateTags를 넘겨 받는다.
bool URStateContext::CanTransition(const FGameplayTagContainer& InContainer)
{
	// 하나라도 있을 경우 취소
	if (InContainer.HasAny(Condition.BlockedTags))
	{
		return false;
	}
	
	// RequiredTags에 모두 포함되어있는지 확인
	if (false == InContainer.HasAll(Condition.RequiredTags))
	{
		return false;
	}
	
	if (InstancePolicy == ERContextInstancePolicty::Single && InContainer.HasAny(Condition.CurrentTags))
	{
		// 이미 실행 중 
		return false;
	}
	
	return true;
}

void URStateContext::ExecuteContext(FGameplayTagContainer& InContainer)
{
	bool bCanExecute = OnEnter();

	if (bCanExecute)
	{
		Execute();
		// 현재 컨테이너에서 InContainer를 뒤에 부착한다.
		InContainer.AppendTags(Condition.CurrentTags);
	}
	
	if (ExecutionType == ERContextExecutionType::Instance)
	{
		// 실행 후 해당 태그 제거
		OnExit();
		InContainer.RemoveTags(Condition.CurrentTags);
	}
}

void URStateContext::OnExit()
{
	if (false == ActionStateComponent.IsValid())
	{
		return;
	}
	
	FGameplayTagContainer& ActiveContainer = ActionStateComponent->GetActiveGameplayTagContainer();
	ActiveContainer.RemoveTags(Condition.CurrentTags);
}

bool URStateContext::IsCancel(const FGameplayTagContainer& CancelContainer)
{
	return Condition.CurrentTags.HasAny(CancelContainer);
}
