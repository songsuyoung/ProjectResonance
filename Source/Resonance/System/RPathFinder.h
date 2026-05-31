#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RPathFinder.generated.h"

enum class ERPatrolPointType : uint8;
USTRUCT(BlueprintType)
struct FRRoutePointContainer
{
	GENERATED_BODY()
public:
	
	UPROPERTY(Transient)
	TArray<FVector> Location;
};

USTRUCT(BlueprintType)
struct FRPathNode
{
	GENERATED_BODY()
public:
	UPROPERTY(Transient)
	TArray<double> Cost;
};

struct FNodeComparator
{
	bool operator()(const TPair<int32, double>& A, const TPair<int32, double>& B) const
	{
		return A.Value < B.Value;
	}
};

UCLASS()
class RESONANCE_API URPathFinder : public UObject
{
	GENERATED_BODY()

public:
	static URPathFinder* Get(const UObject* WorldContextObject);
	// ARPatrolActor을 모두 가져오고 캐싱한다.
	void Init();	
	
	// 현재 캐릭터의 위치 -> 목적지 까지의 위치를 담은 벡터 전달
	TArray<FVector> FindPath(const FVector& StartLocation, const FVector& Destination);
protected:
	
	void MakeGraph();
protected:
	// Spawn에 사용되는 ActorPoint는 이중 관리 필요
	UPROPERTY(Transient)
	TMap<ERPatrolPointType, FRRoutePointContainer> PatrolPoints;
	
	UPROPERTY(Transient)
	TArray<FRPathNode> GraphNode;
	
	UPROPERTY(Transient)
	TArray<FVector> Locations;
};
