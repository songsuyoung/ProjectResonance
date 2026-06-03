#include "NPC/Task/RSTTask_Patrol.h"

// UE 5.
#include "AIController.h"
#include "NavigationSystem.h"
#include "StateTreeExecutionContext.h"
#include "Components/SplineComponent.h"
#include "NavigationPath.h"

//
#include "Character/RBaseCharacter.h"
#include "Data/ResonanceStructs.h"
#include "System/RPathFinder.h"


URSTTask_Patrol::URSTTask_Patrol(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, AcceptableRadius(100.f)
	, SlowDownRadius(300.f)
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
}

EStateTreeRunStatus URSTTask_Patrol::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
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

	const FRPathRoutePayload& Payload = StateTreeEvent->Payload.Get<FRPathRoutePayload>();

	URPathFinder* PathFinder = URPathFinder::Get(this);
	check(PathFinder);
	
	const TArray<FVector>& Locations = PathFinder->FindPath(Payload.NearestPointIndex, Payload.DestinationPointIndex);
	
	SplineComponent->SetSplinePoints(Locations, ESplineCoordinateSpace::World);
	
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

EStateTreeRunStatus URSTTask_Patrol::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	// SplinePoints의 인덱스가 작다면 이동할 위치가 존재함.
	
	// 현재 위치에서 SplinePoint위치까지 Interp 시키면서 이동 
	// Z값은 변하지 않고 X/Y값만 변경
	// Position 값 뿐만 아니라 Rotation값 변경 
	
	// 일정 거리에 도달하면 Index를 올려줌
	if (CurrentSplineIndex >= SplinePoints.Num())
	{
		return EStateTreeRunStatus::Succeeded;
	}

	FVector CurrentLocation = OwnerCharacter->GetActorLocation();
	FVector TargetLocation  = SplinePoints[CurrentSplineIndex].Position;
	TargetLocation.Z        = CurrentLocation.Z; // Z 고정

	// 방향 계산
	FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();

	// Rotation 변경
	FRotator TargetRotation = Direction.Rotation();
	OwnerCharacter->SetActorRotation(
		FMath::RInterpTo(OwnerCharacter->GetActorRotation(), TargetRotation, DeltaTime, 10.f)
	);
	
	float RemainingDistance = FVector::Dist2D(CurrentLocation, TargetLocation);
		
	// 거리를 SlowDownRadius씩 나눈다.
	float ScaleValue = FMath::Clamp(RemainingDistance/SlowDownRadius, 0.f,1.0f);
	
	OwnerCharacter->AddMovementInput(Direction, ScaleValue);

	if (RemainingDistance < AcceptableRadius)
	{
		++CurrentSplineIndex;
	}
	
	return EStateTreeRunStatus::Running;
}
