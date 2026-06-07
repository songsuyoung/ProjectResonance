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
	// Get/Set
	int32 GetNearestNodeIndex(const FVector& TargetLocation);
	int32 PickDestination();
	const TArray<FRPointData>& GetPathPoints() { return Locations; }
	const TArray<int32>& GetTypedPointIndex(ERPathPointType PathPoint) { return TypedPoints[PathPoint].PointIndex; }
	// TODO: Transform으로 해야할 필요가 있을 수도 있음 
	// 이유: 바라보는 방향으로 액터를 스폰해야할 수 있기 때문임.
	FVector GetPathLocation(int32 Index) { return Locations[Index].Tansform.GetLocation(); }
public:
	// 선형적으로 목표에 도달하고 싶을 때 사용하는 함수
	TArray<FVector> FindPath(const int32& StartIndex, const int32& DestinationIndex);
	// 원형으로 돌 필요가 있을 경우 (예 : 공원 산책, 운동장 돌기 등)
	TArray<FVector> FindPath_Circuit(const FVector& StartLocation);

protected:
	bool Dijkstra(int32 StartIndex, int32 EndIndex, TArray<int32>& RoutePathIndex);
	
	TArray<FVector> FindPath_Internal(const int32& StartIndex, const int32& EndIndex);

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float RandomWeight;
	
protected:
	
	UPROPERTY(Transient)
	TMap<ERPathPointType, FRRoutePointContainer> TypedPoints;
	
	UPROPERTY(Transient)
	FRGraph GraphNode;
	
	UPROPERTY(Transient)
	TArray<FRPointData> Locations;
};
