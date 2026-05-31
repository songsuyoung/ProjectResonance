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
			int32 NeighborIndex = Index; // 실제 이웃 노드
			const double NewCost = CurrentNode.Value + GraphNode[CurrentNode.Key].Cost[Index];
    
			if (NewCost >= MAX_FLT || Visited[NeighborIndex])
			{
				continue;
			}
    
			if (Distance[NeighborIndex] > NewCost)
			{
				Previous[NeighborIndex] = CurrentNode.Key;
				Distance[NeighborIndex] = NewCost;
				OpenList.HeapPush(TPair<int32, double>(NeighborIndex, NewCost), FNodeComparator());
			}
		}
	}
	
	// 포함되어있지 않으면, 실패한 경우
	if (Previous[RouteIndex.Value] == -1 && RouteIndex.Key != RouteIndex.Value)
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
		
		UNavigationPath* NavPath = NavigationSystem->FindPathToLocationSynchronously(
			GetWorld(),
			Locations[Index],
			Locations[PrevIndex]
		);
		
		if (IsValid(NavPath))
		{
			RoutePoints.Append(NavPath->PathPoints);
		}
		
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
		GraphNode[RowIndex].Cost.Init(DBL_MAX, Locations.Num());
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
			UNavigationPath* NavPath = NavigationSystem->FindPathToLocationSynchronously(
				World,
				Locations[RowIndex],
				Locations[ColIndex]
			);
			
			float HeightDiff = FMath::Abs(Locations[RowIndex].Z - Locations[ColIndex].Z);
			float HorizontalDist = FVector::Dist2D(Locations[RowIndex], Locations[ColIndex]);

			// 수평 거리가 0이면 나누기 방지
			// Tangent를 수행했을 때 60도 크기는 갈 수 없음으로 인지
			if (HorizontalDist < KINDA_SMALL_NUMBER)
			{
				continue;
			}

			if ((HeightDiff / HorizontalDist) > 1.5f)
			{
				continue;
			}
			
			if (IsValid(NavPath))
			{
				double PathCost = NavPath->GetPathCost();
				
				GraphNode[RowIndex].Cost[ColIndex] = PathCost;
				GraphNode[ColIndex].Cost[RowIndex] = PathCost;
			}
		}
	}
	
	for (int32 i = 0; i < GraphNode.Num(); i++)
	{
		for (int32 j = 0; j < GraphNode[i].Cost.Num(); j++)
		{
			if (GraphNode[i].Cost[j] < MAX_FLT)
			{
				UE_LOG(LogTemp, Warning, TEXT("Edge: %d -> %d, Cost: %f"), i, j, GraphNode[i].Cost[j]);
			}
		}
	}
}

// 다익스트라 - 특정 노드에서 모든 노드까지 최단 비용 반환
TArray<double> URPathFinder::Dijkstra(int32 StartIndex)
{
    int32 NodeCount = Locations.Num();
    TArray<double> Distance;
    Distance.Init(DBL_MAX, NodeCount);
    Distance[StartIndex] = 0.0;

    TArray<bool> Visited;
    Visited.Init(false, NodeCount);

    TArray<TPair<int32, double>> OpenList;
    OpenList.HeapPush({StartIndex, 0.0}, FNodeComparator());

    while (!OpenList.IsEmpty())
    {
        TPair<int32, double> Current;
        OpenList.HeapPop(Current, FNodeComparator());

        if (Visited[Current.Key]) continue;
        Visited[Current.Key] = true;

        for (int32 Index = 0; Index < GraphNode[Current.Key].Cost.Num(); Index++)
        {
            if (GraphNode[Current.Key].Cost[Index] >= DBL_MAX) continue;

            double NewCost = Current.Value + GraphNode[Current.Key].Cost[Index];
            if (Distance[Index] > NewCost)
            {
                Distance[Index] = NewCost;
                OpenList.HeapPush({Index, NewCost}, FNodeComparator());
            }
        }
    }

    return Distance;
}