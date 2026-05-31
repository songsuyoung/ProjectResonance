#include "System/RPathFinder.h"

// UE 5.
#include "EngineUtils.h"
#include "NavigationSystem.h"

// 
#include "NavigationPath.h"
#include "RGameInstance.h"
#include "Actor/RPatrolActor.h"

URPathFinder* URPathFinder::Get(const UObject* WorldContextObject)
{   
	UWorld* World = WorldContextObject->GetWorld();

	check(World);

	URGameInstance* GameInstance = Cast<URGameInstance>(World->GetGameInstance());

	check(GameInstance);

	return GameInstance->GetPathFinder();
}

void URPathFinder::Init()
{
	if (false == PatrolPoints.IsEmpty())
	{
		return;
	}
	
	UWorld* World = GEditor->GetEditorWorldContext().World();

	check(World);
	
	// 배치된 PatrolActor 을 모두 가져온다.
	for (TActorIterator<ARPatrolActor> It(World); It; ++It)
	{
		ARPatrolActor* Actor = *It;
		
		if (IsValid(Actor))
		{
			const ERPatrolPointType PatrolPointType = Actor->GetPatrolPointType();
			const FVector& Location = Actor->GetActorLocation();
			
			// NPC가 임의의 노드 값이 필요할 때 사용
			FRRoutePointContainer& RoutePointContainer = PatrolPoints.FindOrAdd(PatrolPointType);
			RoutePointContainer.Location.Add(Location);
			
			// 노드 정리용도
			Locations.Add(Location);
		}
	}
	
	MakeGraph();
}

TArray<FVector> URPathFinder::FindPath(const FVector& StartLocation, const FVector& Destination)
{
	// 위치를 전달하기 때문에 가장 가까운 노드를 찾아야 다익스트라 알고리즘이 작동 가능하다.
	// 아래 코드는 현재 노드 중에 가장 가까운 노드를 찾는 알고리즘이다.
	float SMin = FLT_MAX;
	float DMin = FLT_MAX;
	TPair<int32, int32> RouteIndex;
	for (int32 Index = 0; Index < Locations.Num(); Index++)
	{
		// 현재 위치와 가장 가까운 위치를 찾는다.
		float SDist = FVector::Dist(Locations[Index], StartLocation);
		float DDist = FVector::Dist(Destination, Locations[Index]);
		
		if (SDist < SMin)
		{
			SMin = SDist;
			RouteIndex.Key = Index;
		}
		
		if (DDist < DMin)
		{
			DMin = DDist;
			RouteIndex.Value = Index;
		}
	}

	TArray<int32> Previous;
	Previous.Init(-1, Locations.Num());
		
	//Dijksta Algorithm
	TArray<double> Distance;
	TArray<TPair<int32, double>> OpenList;
	TArray<bool> Visited;
	Visited.Init(false, Locations.Num());
	
	Distance.Init(MAX_FLT, Locations.Num());
	OpenList.HeapPush({RouteIndex.Key, 0.f},FNodeComparator());
	Distance[RouteIndex.Key] = 0.f;
	
	while (false == OpenList.IsEmpty())
	{
		TPair<int32, double> CurrentNode;
		OpenList.HeapPop(CurrentNode,FNodeComparator());
		
		// 이전에 저장된 값보다 현재 따로 저장한 값이 크면, 아래를 진행할 필요가 없음
		if (Visited[CurrentNode.Key])
		{
			continue;
		}
		
		if (CurrentNode.Key == RouteIndex.Value)
		{
			break;
		}
		
		Visited[CurrentNode.Key] = true;
		
		for (int32 Index = 0; Index < GraphNode[CurrentNode.Key].Cost.Num(); Index++)
		{
			const double NewCost = CurrentNode.Value + GraphNode[CurrentNode.Key].Cost[Index];
			
			// 갈 곳이 없음.
			if (NewCost >= MAX_FLT || Visited[Index])
			{
				continue;
			}
			
			if (Distance[Index] > NewCost)
			{
				Previous[Index] = CurrentNode.Key;
				Distance[Index] = NewCost;
				OpenList.HeapPush(TPair<int32, double>(Index, Index),FNodeComparator());
			}
		}
	}
	
	// 포함되어있지 않으면, 실패한 경우
	if (false == Previous.Contains(RouteIndex.Value))
	{
		return TArray<FVector>();
	}
	
	TArray<FVector> RoutePoints;
	int32 Index = RouteIndex.Value;
	
	UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	
	check(NavigationSystem);
	while (RouteIndex.Key != Index)
	{
		int32 PrevIndex = Previous[Index];
		if (PrevIndex == -1) break;
		
		/*UNavigationPath* NavPath = NavigationSystem->FindPathToLocationSynchronously(
			GetWorld(),
			Locations[Index],
			Locations[PrevIndex]
		);
		
		if (IsValid(NavPath))
		{
			RoutePoints.Append(NavPath->PathPoints);
		}*/
		RoutePoints.Add(Locations[Index]);
		
		Index = PrevIndex;	
	}
	RoutePoints.Add(Locations[Index]);
	
	Algo::Reverse(RoutePoints);
	
	return RoutePoints;
}

void URPathFinder::MakeGraph()
{
	GraphNode.SetNum(Locations.Num());
	for (int32 RowIndex = 0; RowIndex < Locations.Num(); RowIndex++)
	{
		GraphNode[RowIndex].Cost.Init(FLT_MAX, Locations.Num());
	}
	
	UWorld* World = GEditor->GetEditorWorldContext().World();

	check(World);
	
	UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetNavigationSystem(World);
	
	check(NavigationSystem);
	
	// 비용은 모두 반영되기 때문에, O^2로 할 필요 없음 절반 줄일 수 있음.
	// 2차원 배열
	for (int RowIndex = 0; RowIndex < Locations.Num(); RowIndex++)
	{
		for (int ColIndex = RowIndex + 1; ColIndex < Locations.Num(); ColIndex++)
		{
			// 네비게이션 시스템을 사용해서 현재 위치에서 다음 위치로 갈 수 있는지 여부를 확인
			// 갈 수 없다면 FLT_MAX 취급
			ANavigationData* NavData = NavigationSystem->GetDefaultNavDataInstance();

			// 실제 경로 비용이 필요한 경우
			FPathFindingQuery Query(nullptr, *NavData, Locations[RowIndex], Locations[ColIndex]);
			FPathFindingResult PathResult = NavigationSystem->FindPathSync(Query);

			if (PathResult.IsSuccessful())
			{
				float PathCost = PathResult.Path->GetCost();
				GraphNode[RowIndex].Cost[ColIndex] = PathCost;
				GraphNode[ColIndex].Cost[RowIndex] = PathCost;
			}
		}
	}
}
