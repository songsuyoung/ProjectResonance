#pragma once

#include "CoreMinimal.h"
#include "Data/RCoreStructs.h"
#include "UObject/NoExportTypes.h"
#include "RPathFinder.generated.h"

enum class ERPathPointType : uint8;
USTRUCT()
struct FRRoutePointContainer
{
	GENERATED_BODY()
public:
	
	UPROPERTY(Transient)
	TArray<int32> PointIndex;
};

USTRUCT()
struct FRNodeComparator
{
	GENERATED_BODY()
public:
	bool operator()(const FRPathEdge& A, const FRPathEdge& B) const
	{
		return A.Cost < B.Cost;
	}
};

UCLASS()
class RESONANCE_API URPathFinder : public UObject
{
	GENERATED_BODY()

public:
	URPathFinder();
	static URPathFinder* Get(const UObject* WorldContextObject);
	void Initialize();
public:
	int32 GetNearestNodeIndex(const FVector& TargetLocation);
	int32 PickDestination();
	
	TArray<FVector> FindPath(const FVector& StartLocation, const FVector& Destination);
	TArray<FVector> FindPath(const int32& StartIndex, const int32& DestinationIndex);
	
protected:
	bool Dijkstra(int32 StartIndex, int32 EndIndex, TArray<int32>& RoutePathIndex);
	TArray<FVector> FindPath_Internal(const int32& StartIndex, const int32& EndIndex);

protected:
	
	UPROPERTY(Transient)
	TMap<ERPathPointType, FRRoutePointContainer> TypedPatrolPoints;
	
	UPROPERTY(Transient)
	FRGraph GraphNode;
	
	UPROPERTY(Transient)
	TArray<FVector> Locations;
};
