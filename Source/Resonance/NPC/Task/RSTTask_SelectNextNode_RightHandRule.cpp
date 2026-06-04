#include "RSTTask_SelectNextNode_RightHandRule.h"

// UE
#include "StateTreeExecutionContext.h"

// 
#include "System/RPathFinder.h"

URSTTask_SelectNextNode_RightHandRule::URSTTask_SelectNextNode_RightHandRule(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, NearbyRadius(300.f)
{
}

EStateTreeRunStatus URSTTask_SelectNextNode_RightHandRule::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	URPathFinder* PathFinder = URPathFinder::Get(this);

	check(PathFinder);
	
	// 현재 액터 위치를 가져온다.
	const TArray<FVector>& Locations = PathFinder->GetPathPoints();

	TArray<TPair<int32, FVector>> NearestLocations;
	// 현재 액터에게 가까운 노드들을 가져온다.
	for (int32 Index = 0; Index < Locations.Num(); Index++)
	{
		//가장 가까운 노드들 평가
		float Distance = FVector::Dist(Locations[Index], OwnerCharacter->GetActorLocation());

		if (Distance < NearbyRadius)
		{
			NearestLocations.Add({Index,Locations[Index]});
			DrawDebugSphere(GetWorld(), Locations[Index], 20.f, 8, FColor::Black, false, 2.f);
		}
	}
		
	TArray<TPair<int32, double>> ConnectedRoutes;
	
	// 노드들로 부터 왼쪽/오른쪽을 구분하고. 오른쪽을 선택한다.
	for (int32 Index = 0; Index < NearestLocations.Num(); Index++)
	{
		FVector Dir = (NearestLocations[Index].Value - OwnerCharacter->GetActorLocation()).GetSafeNormal();
		FVector Forward = OwnerCharacter->GetActorForwardVector();

		// 두 외적을 통해 양수인지 음수인지를 평가 (Z값이)
		FVector Result = FVector::CrossProduct(Forward, Dir);

		// UpVector 내적을 통해서 양수인이 아닌지 평가
		float DotProduct= FVector::DotProduct(Result,  OwnerCharacter->GetActorUpVector());

		if (DotProduct > 0.f)  // >= 0 에서 > 0 으로
		{
			float CompareValue = FVector::DotProduct(Dir, Forward);
			ConnectedRoutes.HeapPush({NearestLocations[Index].Key, CompareValue}, FRRouteComparator());
			DrawDebugSphere(GetWorld(), NearestLocations[Index].Value, 20.f, 8, FColor::Black, false, 2.f);
		}
	}

	int32 SelectedIndex = -1;
	
	// 우측 보행의 노드를 먼저 선택 => 그 노드를 같은 방향으로 이동
	if (ConnectedRoutes.IsEmpty())
	{
		// 그런데, 만약 우측 보행 노드가 존재하지 않은 경우 가장 가까운 노드를 선택한다.
		SelectedIndex = PathFinder->GetNearestNodeIndex(OwnerCharacter->GetActorLocation());	
	}
	else
	{
		TPair<int32, double> MaxCostNode;
		ConnectedRoutes.HeapPop(MaxCostNode, FRRouteComparator());
		SelectedIndex = MaxCostNode.Key;
	}
	DrawDebugSphere(GetWorld(), Locations[SelectedIndex], 20.f, 8, FColor::Cyan, false, 2.f);
	
	return Super::EnterState(Context, Transition);
}
