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
	, RandomWeight(0.6f)
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
		Locations.Add(PathDataArray.TransformData[Index]);
		FRRoutePointContainer& RoutePointContainer = TypedPoints.FindOrAdd(PathDataArray.TransformData[Index].PathPointType);
		
		RoutePointContainer.PointIndex.Add(Index);
	}
	
	FString OutResult;
    
	FRFileHelper::ReadStringFromJson(TEXT("/GraphNodeData.json"), OutResult);
	FJsonObjectConverter::JsonObjectStringToUStruct(OutResult, &GraphNode);
}

int32 URPathFinder::GetNearestNodeIndex(const FVector& TargetLocation)
{
	// 우측 Bias값을 사용해서, 일부 랜덤으로 우측을 기준으로 좌측을 기준으로를 정하자.
	TArray<int32> NearestNodes;
	for (int32 Index = 0; Index < Locations.Num(); Index++)
	{
		// 현재 위치와 가장 가까운 위치를 찾는다.
		float Dist = FVector::Dist(Locations[Index].Tansform.GetLocation(), TargetLocation);
		
		// 임시값 사용
		if (Dist < 500.f)
		{
			NearestNodes.Add(Index);
		}
	}
	
	int32 RandIndex = FMath::RandRange(0, NearestNodes.Num() - 1);
	
	return NearestNodes[RandIndex];
}

bool URPathFinder::Dijkstra(int32 StartIndex, int32 EndIndex, TArray<int32>& RoutePathIndex)
{
	// 0~1 사이의 값 추출 *0.3하면 0~0.3 값을 가질 수 있음.
	float RightWeight = FMath::FRand() < RandomWeight ? 0.7f : -0.7f;

	RoutePathIndex.Empty();
	RoutePathIndex.Init(-1, Locations.Num());

	//Dijksta Algorithm
	TArray<double> Distance;
	TArray<FRPathEdge> OpenList;

	Distance.Init(DBL_MAX, Locations.Num());

	OpenList.HeapPush({StartIndex, 0.f, FVector::ForwardVector}, FRNodeComparator());
	Distance[StartIndex] = 0.f;

	while (false == OpenList.IsEmpty())
	{
		FRPathEdge CurrentNode;
		OpenList.HeapPop(CurrentNode, FRNodeComparator());

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

			FVector NextDir = (Locations[NextEdge.Index].Tansform.GetLocation() - Locations[CurrentNode.Index].Tansform.
				GetLocation()).GetSafeNormal();

			FVector Cross = FVector::CrossProduct(CurrentNode.CurrentForward, NextDir);
			// 왼손 좌표계로, 음수일때 오른쪽을 의미함.
			// 좌측일 수록 양수
			// Z축이 클수록 90도에 가까움. 
			float RightBias = FMath::Clamp(-Cross.Z, -1.f, 1.0f);

			// 비율을 가중치로 두면, 거리 상관 없이 30프로 비용 차감
			float NewCost = (CurrentNode.Cost + NextEdge.Cost) - (RightBias * NextEdge.Cost * RightWeight);

			if (Distance[NextEdge.Index] > NewCost)
			{
				RoutePathIndex[NextEdge.Index] = CurrentNode.Index;
				Distance[NextEdge.Index] = NewCost;
				OpenList.HeapPush({NextEdge.Index, NewCost, NextDir}, FRNodeComparator());
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

TArray<FVector> URPathFinder::FindPath(const int32& StartIndex, const int32& DestinationIndex)
{
	const FRRoutePointContainer* RoutePointContainer = TypedPoints.Find(ERPathPointType::Destination);

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
	
	// Dijkstra 알고리즘을 진행한다.
	bool PathResult = Dijkstra(StartIndex, EndIndex, RoutePathIndex);
	
	if (false == PathResult)
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
			Locations[Index].Tansform.GetLocation(),
			Locations[PrevIndex].Tansform.GetLocation()
		);
		
		if (IsValid(NavPath))
		{
			RoutePoints.Append(NavPath->PathPoints);
		}
		
		Index = PrevIndex;	
	}
	
	RoutePoints.Add(Locations[Index].Tansform.GetLocation());
	
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
	FVector NextLocation = Locations[RealDirIndex].Tansform.GetLocation();
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
			
			FVector CandidateLocation  = Locations[CandidateIndex].Tansform.GetLocation();
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
	FRRoutePointContainer* RoutePointContainer = TypedPoints.Find(ERPathPointType::Destination);

	if (nullptr == RoutePointContainer)
	{
		return -1;	
	}

	int LastIndex = RoutePointContainer->PointIndex.Num();

	return FMath::RandRange(0, LastIndex - 1);
}
