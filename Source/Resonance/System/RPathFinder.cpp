#include "System/RPathFinder.h"

// UE 5.
#include "NavigationSystem.h"
#include "JsonObjectConverter.h"
#include "NavigationPath.h"

// 
#include "RBakeDataManager.h"
#include "RGameInstance.h"
#include "Data/RCoreEnums.h"
#include "System/RFileHelper.h"

URPathFinder::URPathFinder()
	: Super()
{
}

URPathFinder* URPathFinder::Get(const UObject* WorldContextObject)
{   
	UWorld* World = WorldContextObject->GetWorld();

	check(World);

	URGameInstance* GameInstance = Cast<URGameInstance>(World->GetGameInstance());

	check(GameInstance);

	return GameInstance->GetPathFinder();
}

void URPathFinder::Initialize()
{
	URBakeDataManager* BakeDataManager = URBakeDataManager::Get(this);
	
	check(BakeDataManager);
	
	const FRBakeDataGroup& BakeDataGroup = BakeDataManager->GetBakeDataGroup();
	
	if (false == BakeDataGroup.BakeDataMap.Contains(ERBakeType::PathPoint))
	{
		return;
	}
	
	const FRTransformDataArray& PathDataArray = BakeDataGroup.BakeDataMap[ERBakeType::PathPoint];
	
	for (int32 Index = 0; Index < PathDataArray.TransformData.Num(); Index++)
	{
		Locations.Add(PathDataArray.TransformData[Index].Tansform.GetLocation());
		FRRoutePointContainer& RoutePointContainer = TypedPatrolPoints.FindOrAdd(PathDataArray.TransformData[Index].PathPointType);
		
		RoutePointContainer.PointIndex.Add(Index);
	}
	
	FString OutResult;
    
	FRFileHelper::ReadStringFromJson(TEXT("/GraphNodeData.json"), OutResult);
	FJsonObjectConverter::JsonObjectStringToUStruct(OutResult, &GraphNode);
}

int32 URPathFinder::GetNearestNodeIndex(const FVector& TargetLocation)
{
	double Min = DBL_MAX;
	int32 NearestNodeIndex = -1;

	for (int32 Index = 0; Index < Locations.Num(); Index++)
	{
		// 현재 위치와 가장 가까운 위치를 찾는다.
		float Dist = FVector::Dist(Locations[Index], TargetLocation);
		
		if (Dist < Min)
		{
			Min = Dist;
			NearestNodeIndex = Index;
		}
	}

	return NearestNodeIndex;
}

bool URPathFinder::Dijkstra(int32 StartIndex, int32 EndIndex, TArray<int32>& RoutePathIndex)
{
	RoutePathIndex.Empty();
	RoutePathIndex.Init(-1, Locations.Num());
		
	//Dijksta Algorithm
	TArray<double> Distance;
	TArray<FRPathEdge> OpenList;
	
	Distance.Init(DBL_MAX, Locations.Num());
	
	OpenList.HeapPush({StartIndex, 0.f},FRNodeComparator());
	Distance[StartIndex] = 0.f;
	
	while (false == OpenList.IsEmpty())
	{
		FRPathEdge CurrentNode;
		OpenList.HeapPop(CurrentNode,FRNodeComparator());
		
		// 이전에 저장된 값보다 현재 따로 저장한 값이 크면, 아래를 진행할 필요가 없음
		if (Distance[CurrentNode.Index] < CurrentNode.Cost)
		{
			continue;
		}
		
		if (CurrentNode.Index == EndIndex)
		{
			break;
		}
		
		for (int32 Index = 0; Index < GraphNode.AdjacencyList[CurrentNode.Index].Edges.Num(); Index++)
		{
			const FRPathEdge& NextEdge = GraphNode.AdjacencyList[CurrentNode.Index].Edges[Index]; // 실제 이웃 노드
			float NewCost = CurrentNode.Cost + NextEdge.Cost;
			
			if (Distance[NextEdge.Index] > NewCost)
			{
				RoutePathIndex[NextEdge.Index] = CurrentNode.Index;
				Distance[NextEdge.Index] = NewCost;
				OpenList.HeapPush({NextEdge.Index, NewCost}, FRNodeComparator());
			}
		}
	}
	
	// 포함되어있지 않으면, 실패한 경우
	if (RoutePathIndex[EndIndex] == -1 && EndIndex != StartIndex)
	{
		RoutePathIndex.Empty();
		return false;
	}

	return true;
}

TArray<FVector> URPathFinder::FindPath(const FVector& StartLocation, const FVector& Destination)
{
	// 위치를 전달하기 때문에 가장 가까운 노드를 찾아야 다익스트라 알고리즘이 작동 가능하다.
	// 아래 코드는 현재 노드 중에 가장 가까운 노드를 찾는 알고리즘이다.
	int32 StartIndex = GetNearestNodeIndex(StartLocation);
	int32 EndIndex = GetNearestNodeIndex(Destination);

	return FindPath(StartIndex, EndIndex);
}

TArray<FVector> URPathFinder::FindPath(const int32& StartIndex, const int32& DestinationIndex)
{
	const FRRoutePointContainer* RoutePointContainer = TypedPatrolPoints.Find(ERPathPointType::Destination);

	if (nullptr == RoutePointContainer)
	{
		return TArray<FVector>();
	}

	if (false == RoutePointContainer->PointIndex.IsValidIndex(DestinationIndex))
	{
		return TArray<FVector>();
	}
	
	int EndIndex = RoutePointContainer->PointIndex[DestinationIndex];
	
	return FindPath_Internal(StartIndex, EndIndex);
}

TArray<FVector> URPathFinder::FindPath_Internal(const int32& StartIndex, const int32& EndIndex)
{
	TArray<int32> RoutePathIndex;
	bool bResult = Dijkstra(StartIndex, EndIndex, RoutePathIndex);

	if (false == bResult)
	{
		return TArray<FVector>();
	}
	
	TArray<FVector> RoutePoints;
	UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetNavigationSystem(this);
	
	check(NavigationSystem);

	int32 Index = EndIndex;
	while (StartIndex != Index)
	{
		int32 PrevIndex = RoutePathIndex[Index];
		if (PrevIndex == -1) break;
		
		UNavigationPath* NavPath = NavigationSystem->FindPathToLocationSynchronously(
			this,
			Locations[Index],
			Locations[PrevIndex]
		);
		
		if (IsValid(NavPath))
		{
			RoutePoints.Append(NavPath->PathPoints);
		}
		
		Index = PrevIndex;	
	}
	
	RoutePoints.Add(Locations[Index]);
	
	Algo::Reverse(RoutePoints);
	
	return RoutePoints;
}

TArray<FVector> URPathFinder::FindPath_Circuit(const FVector& StartLocation)
{
	TArray<FVector> Path;
	// 가장 가까운 노드 인덱스를 가져온다.
	int32 StartIndex = GetNearestNodeIndex(StartLocation);

	int32 NextIndex = FMath::RandRange(0, GraphNode.AdjacencyList[StartIndex].Edges.Num() - 1);
	int32 RealDirIndex = GraphNode.AdjacencyList[StartIndex].Edges[NextIndex].Index; //실제 인덱스가 저장되어있음
	if (false == Locations.IsValidIndex(RealDirIndex))
	{
		return Path;
	}
	
	int32 PrevIndex = StartIndex;
	// Direction과 가장 일치하는 방향으로 이동한다.
	FVector NextLocation = Locations[RealDirIndex];
	FVector Direction = (NextLocation- StartLocation).GetSafeNormal();
	
	Path.Add(NextLocation);
	
	while (RealDirIndex != StartIndex)
	{
		float MaxDot = -1.f;
		
		for (int32 Index = 0; Index < GraphNode.AdjacencyList[RealDirIndex].Edges.Num(); Index++)
		{
			int32 CandidateIndex =  GraphNode.AdjacencyList[RealDirIndex].Edges[Index].Index; //실제 인덱스가 저장되어있음
			if (false == Locations.IsValidIndex(CandidateIndex))
			{
				return TArray<FVector>();
			}
			
			// 이전으로 돌아가지 않도록 한다.
			if (CandidateIndex == PrevIndex)
			{
				continue;
			}
			
			FVector CandidateLocation  = Locations[CandidateIndex];
			FVector NextDir = (CandidateLocation - Path.Last()).GetSafeNormal();
			// Direction과 가장 일치하는 방향으로 이동한다.
			float Result = FVector::DotProduct(Direction, NextDir);
			
			if (MaxDot < Result)
			{
				MaxDot = Result;
				RealDirIndex = CandidateIndex;
				NextLocation = CandidateLocation;
			}
		}

		// 모든 과정이 끝나고, 다음 이동 할 때 
		if (Path.Contains(NextLocation))
		{
			return TArray<FVector>();
		}
		
		PrevIndex = RealDirIndex;
		Direction = (NextLocation - Path.Last()).GetSafeNormal();
		Path.Add(NextLocation);
	}

	return Path;
}

int32 URPathFinder::PickDestination()
{
	FRRoutePointContainer* RoutePointContainer = TypedPatrolPoints.Find(ERPathPointType::Destination);

	if (nullptr == RoutePointContainer)
	{
		return -1;	
	}

	int LastIndex = RoutePointContainer->PointIndex.Num();

	return FMath::RandRange(0, LastIndex - 1);
}
