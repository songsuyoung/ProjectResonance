#include "RSTTask_Talk.h"

// UE 5.
#include "StateTreeExecutionContext.h"
#include "Engine/OverlapResult.h"

//
#include "Character/RNPCCharacter.h"

URSTTask_Talk::URSTTask_Talk(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bTalk(false)
{
}

EStateTreeRunStatus URSTTask_Talk::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	// 오버랩을 만든다.
	UWorld* World = Context.GetWorld();
	
	check(World);
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionParams;

	if (false == OwnerCharacter.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}
	
	CollisionParams.AddIgnoredActor(OwnerCharacter.Get());

	FVector CenterLocation = OwnerCharacter->GetActorLocation();
	
	bool bRes = World->OverlapMultiByChannel(
		OverlapResults,
		CenterLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_Visibility,
		FCollisionShape::MakeSphere(SearchRange),
		CollisionParams
	);
	
	if (bRes)
	{
		float MinDistance = FLT_MAX;
		// 주변에 대화할 사람이 있니?
		for (const FOverlapResult& OverlapResult : OverlapResults)
		{
			ACharacter* TalkTarget = Cast<ACharacter>(OverlapResult.GetActor());
			
			if (IsValid(TalkTarget))
			{
				// 거리가 가장 가까워야함.
				// 거리 비교
				float Distance = FVector::Dist2D(CenterLocation, TalkTarget->GetActorLocation());
				
				if (MinDistance > Distance)
				{
					// TargetCharacter 설정
					// 성공 => 추후 친분 관계도 설정 예정.
					TargetCharacter = TalkTarget;
					MinDistance = Distance;
				}
	
				// 있으면 bTalk = true
				bTalk = true;
			}
		}
	}
	
	if (bTalk)
	{
		return EStateTreeRunStatus::Succeeded;	
	}
	
	return EStateTreeRunStatus::Failed;
}
