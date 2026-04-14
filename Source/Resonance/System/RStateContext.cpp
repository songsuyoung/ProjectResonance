#include "System/RStateContext.h"

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
	
	return true;
}

void URStateContext::ExecuteContext(FGameplayTagContainer& InContainer)
{
	if (false == InContainer.IsEmpty())
	{
		// CancelTags를 모두 취소한다.
		InContainer.RemoveTags(Condition.CancelTags);
	}
	
	if (InContainer.HasAny(Condition.CurrentTags))
	{
		//모두 취소
		InContainer.RemoveTags(Condition.CurrentTags);
	}
	Execute();
	
	// 현재 컨테이너에서 InContainer를 뒤에 부착한다.
	InContainer.AppendTags(Condition.CurrentTags);
}

AActor* URStateContext::GetOuterActor()
{
	UObject* Outer = GetOuter();
	
	if (false == IsValid(Outer))
	{
		return nullptr;
	}
	AActor* OuterActor = Cast<AActor>(Outer);
	
	if (false == IsValid(OuterActor))
	{
		return nullptr;
	}
	
	return OuterActor;
}

FVector URStateContext::GetOuterLocation()
{
	UObject* Outer = GetOuter();
	
	if (false == IsValid(Outer))
	{
		return FVector::ZeroVector;
	}
	AActor* OuterActor = Cast<AActor>(Outer);
	
	if (false == IsValid(OuterActor))
	{
		return FVector::ZeroVector;
	}
	
	return OuterActor->GetActorLocation();
}
