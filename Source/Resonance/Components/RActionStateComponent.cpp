#include "Components/RActionStateComponent.h"

// New Created Files..
#include "System/RStateContext.h"

URActionStateComponent::URActionStateComponent()
{
}

void URActionStateComponent::PushState(const ERActionContext& InputState)
{
	// 생성 지연
	if (false == ActionContexts.Contains(InputState))
	{
		TSubclassOf<URStateContext>* StateContextClass = ActionContextClass.Find(InputState);
		
		if (nullptr != StateContextClass)
		{
			URStateContext* NewStateContext = NewObject<URStateContext>(GetOwner(), *StateContextClass);
			ActionContexts.Add(InputState, NewStateContext);
		}
	}
	
	bool bResult = FindState(InputState);
	
	if (bResult)
	{
		// Push 가능함을 확인
		// ActionContext가 가지고 있는 데이터를 Container에 저장
		ActionContexts[InputState]->ExecuteContext(Container);
		RegisterGameplayTagEvent(InputState).Broadcast();
		
		// EventMap 취소된 상태값 => Remove 
		// EventMap 성공한 상태값 => Add 
		// 전송해야한다.
	}
}

bool URActionStateComponent::FindState(const ERActionContext& InputState)
{
	if (false == ActionContexts.Contains(InputState))
	{
		return false;
	}
	
	return ActionContexts[InputState]->CanTransition(Container);
}

FOnGameplayTagChanged& URActionStateComponent::RegisterGameplayTagEvent(const ERActionContext& InputState)
{
	FRDelegateInfo& DelegateInfo = EventMap.FindOrAdd(InputState);
	
	return DelegateInfo.OnNewOrRemove;
}