#include "Components/RActionStateComponent.h"

// New Created Files..
#include "StateContext/RStateContext.h"

URActionStateComponent::URActionStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URActionStateComponent::PushState(const ERActionContext& InputState)
{
	// 생성 지연
	if (false == ActionContexts.Contains(InputState))
	{
		FRStateContextClassContainer* NewStateContextClassContainer =  ActionContextClass.Find(InputState);
		
		if (nullptr != NewStateContextClassContainer)
		{
			FRStateContextContainer NewStateContextContainer;
			
			for (const auto& NewStateActionContextClass : NewStateContextClassContainer->ActionContextClasses)
			{
				URStateContext* NewStateContext = NewObject<URStateContext>(GetOwner(), *NewStateActionContextClass);
				NewStateContext->Initialize(this);
				NewStateContextContainer.ActionContexts.Add(NewStateContext);
			}
			
			ActionContexts.FindOrAdd(InputState, NewStateContextContainer);
		}
	}
	
	URStateContext* ActivatableStatContext = FindState(InputState);
	
	if (nullptr != ActivatableStatContext)
	{
		// Push 가능함을 확인
		// ActionContext가 가지고 있는 데이터를 Container에 저장
		TArray<URStateContext*> RemoveActionContexts;
		for (const auto& ActionContext : CurrentActionContexts)
		{
			if (false == ActionContext.IsValid())
			{
				continue;
			}
			
			bool bCanCancel = ActionContext->IsCancel(ActivatableStatContext->GetCancelTags());
			
			if (bCanCancel)
			{
				ActionContext->OnExit();
				RemoveActionContexts.Add(ActionContext.Get());
			}
		}
		
		for (auto& RemoveActionContext : RemoveActionContexts)
		{
			CurrentActionContexts.RemoveSwap(RemoveActionContext);
		}
		
		// Container에서 취소하면 안되고, 실제 CancelTag를 CurrentTag로 가지고 있는 모든 ActionContext를 가져와서 취소하도록 변경해야함.
		ActivatableStatContext->ExecuteContext(Container);
		CurrentActionContexts.Add(ActivatableStatContext);
		RegisterGameplayTagEvent(InputState).Broadcast();
	}
}

void URActionStateComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (const auto& ActionContext : CurrentActionContexts)
	{
		if (false == ActionContext.IsValid())
		{
			continue;
		}
		
		if (ActionContext->CanTick())
		{
			ActionContext->OnTick(DeltaTime);
		}
	}
}

URStateContext* URActionStateComponent::FindState(const ERActionContext& InputState)
{
	if (false == ActionContexts.Contains(InputState))
	{
		return nullptr;
	}
	
	for (const auto& ActionContext : ActionContexts[InputState].ActionContexts)
	{
		if (ActionContext->CanTransition(Container))
		{
			UE_LOG(LogTemp, Log, TEXT("[Skill|ActionState] %s"), *Container.ToString());
			return ActionContext;
		}
	}
	
	return nullptr;
}

FOnGameplayTagChanged& URActionStateComponent::RegisterGameplayTagEvent(const ERActionContext& InputState)
{
	FRDelegateInfo& DelegateInfo = EventMap.FindOrAdd(InputState);
	
	return DelegateInfo.OnNewOrRemove;
}