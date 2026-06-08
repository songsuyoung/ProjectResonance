#include "RSTTask_MoveToSpline.h"

// UE 5.
#include "Components/SplineComponent.h"
#include "StateTreeExecutionContext.h"

//
#include "Character/RNPCCharacter.h"
#include "Data/ResonanceStructs.h"

URSTTask_MoveToSpline::URSTTask_MoveToSpline(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

EStateTreeRunStatus URSTTask_MoveToSpline::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	// 전달받은 데이터를 읽는다.
	// 전달된 데이터는 TArray<FVector> 값을 전달한다.
	// FindLinePath는 목적지에 맞는 직선을 결정
	// FindCircuitPath는 원형으로 전달된 경로를 전달
	
	const FStateTreeEventQueue& EventQueue = Context.GetEventQueue();
	
	const FStateTreeEvent* StateTreeEvent = nullptr;
	
	for (const auto QueueEntry : EventQueue.GetEventsView())
	{
		if (QueueEntry->Tag == FGameplayTag::RequestGameplayTag(FName("StateTree.Task.Event.DestinationDecided")))
		{
			StateTreeEvent = QueueEntry.Get();
			break;
		}
	}
	
	if (nullptr == StateTreeEvent)
	{
		return EStateTreeRunStatus::Failed;
	}
	
	// 초기화
	SplineComponent = OwnerCharacter->GetSplineComponent();

	if (false == SplineComponent.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}
	
	SplineComponent->ClearSplinePoints();
	
	const FRPathRoutePayload& Payload = StateTreeEvent->Payload.Get<FRPathRoutePayload>();
	SplineComponent->SetSplinePoints(Payload.PathLocation, ESplineCoordinateSpace::World);
	for (int32 Index = 0; Index < SplineComponent->GetNumberOfSplinePoints(); ++Index)
	{
		SplineComponent->SetSplinePointType(Index, ESplinePointType::Curve, false);
	}
	SplineComponent->UpdateSpline();
	
	if (bDebugDraw)
	{
		const int32 SampleCount = 100; // 촘촘할수록 곡선처럼 보임
		const float TotalLength = SplineComponent->GetSplineLength();
		const float Step = TotalLength / SampleCount;

		for (int32 Index = 0; Index < SampleCount; Index++)
		{
			FVector Start = SplineComponent->GetLocationAtDistanceAlongSpline(Step * Index, ESplineCoordinateSpace::World);
			FVector End = SplineComponent->GetLocationAtDistanceAlongSpline(Step * (Index + 1), ESplineCoordinateSpace::World);

			DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 10.f, 0, 2.f);
		}

		// 포인트 위치도 같이 표시
		for (int32 Index = 0; Index < SplineComponent->GetNumberOfSplinePoints(); Index++)
		{
			FVector PointLocation = SplineComponent->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);
			DrawDebugSphere(GetWorld(), PointLocation, 20.f, 8, FColor::Green, false, 10.f);
		}	
	}
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus URSTTask_MoveToSpline::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	if (false == SplineComponent.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}

	FVector CurrentLocation = OwnerCharacter->GetActorLocation();
	const float SplineLength = SplineComponent->GetSplineLength();

	// 현재 위치를 스플라인 위의 거리로 투영
	float CurrentDistance = SplineComponent->GetDistanceAlongSplineAtLocation(CurrentLocation, ESplineCoordinateSpace::World);

	// 종료 판정: 스플라인 끝 도달
	float RemainingDistance = SplineLength - CurrentDistance;
	if (RemainingDistance < AcceptableRadius)
	{
		return EStateTreeRunStatus::Succeeded;
	}

	// 현재 위치에서 Nm 앞에 있는 지점 까지 간다.
	// Min인 이유는 SplineLength의 길이를 넘으면 안되기 때문임.
	// 꼭지점이 아니라, 미래에 거리를 기반으로 곡선을 부드럽게 추정
	float TargetDistance = FMath::Min(CurrentDistance + LookAhead, SplineLength);
	FVector TargetLocation = SplineComponent->GetLocationAtDistanceAlongSpline(TargetDistance, ESplineCoordinateSpace::World);
	TargetLocation.Z = CurrentLocation.Z;

	// 방향 계산
	FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();

	// 회전
	FRotator TargetRotation = Direction.Rotation();
	OwnerCharacter->SetActorRotation(FMath::RInterpTo(OwnerCharacter->GetActorRotation(), TargetRotation, DeltaTime, 10.f));
	
	// 감속 코드
	float ScaleValue = FMath::Clamp(RemainingDistance / SlowDownRadius, 0.f, 1.0f);

	OwnerCharacter->AddMovementInput(Direction, ScaleValue);

	return EStateTreeRunStatus::Running;
}
