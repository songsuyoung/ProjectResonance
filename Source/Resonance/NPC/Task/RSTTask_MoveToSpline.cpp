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
	SplinePoints.Empty();
	CurrentSplineIndex = 0;
	SplineComponent = OwnerCharacter->GetSplineComponent();

	if (false == SplineComponent.IsValid())
	{
		return EStateTreeRunStatus::Failed;
	}
	
	const FRPathRoutePayload& Payload = StateTreeEvent->Payload.Get<FRPathRoutePayload>();
	
	SplineComponent->SetSplinePoints(Payload.PathLocation, ESplineCoordinateSpace::World);
	
	// SplineComponent로부터 가져온다.
	for (int32 Index = 0; Index < SplineComponent->GetNumberOfSplinePoints(); Index++)
	{
		SplinePoints.Add(SplineComponent->GetSplinePointAt(Index, ESplineCoordinateSpace::World));
	}
	
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
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus URSTTask_MoveToSpline::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	// 데이터에 따라 진행한다.
	if (CurrentSplineIndex >= SplinePoints.Num())
	{
		return EStateTreeRunStatus::Succeeded;
	}

	FVector CurrentLocation = OwnerCharacter->GetActorLocation();
	FVector TargetLocation  = SplinePoints[CurrentSplineIndex].Position;
	TargetLocation.Z = CurrentLocation.Z; // Z 고정

	// 방향 계산
	FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();

	// Rotation 변경
	FRotator TargetRotation = Direction.Rotation();
	OwnerCharacter->SetActorRotation(
		FMath::RInterpTo(OwnerCharacter->GetActorRotation(), TargetRotation, DeltaTime, 10.f)
	);
	
	float RemainingDistance = FVector::Dist2D(CurrentLocation, TargetLocation);
	// 거리를 SlowDownRadius씩 나눈다.
	float ScaleValue = 1.0f;
	
	// 거의 끝에 도달하면 속도를 줄여야한다.
	if (CurrentSplineIndex >= SplinePoints.Num() - 1)
	{
		ScaleValue = FMath::Clamp(RemainingDistance/SlowDownRadius, 0.f,1.0f);
	}
	
	OwnerCharacter->AddMovementInput(Direction, ScaleValue);

	if (RemainingDistance < AcceptableRadius)
	{
		++CurrentSplineIndex;
	}
	
	return EStateTreeRunStatus::Running;
}
