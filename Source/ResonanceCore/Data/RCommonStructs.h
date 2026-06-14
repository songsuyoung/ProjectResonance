#pragma once
#include "CoreMinimal.h"
#include "RCommonStructs.generated.h"

enum class ERBakeType : uint8;
enum class ERPathPointType : uint8;

USTRUCT()
struct RESONANCECORE_API FRPointData
{
	GENERATED_BODY()
public:
	
	UPROPERTY()
	ERPathPointType PathPointType;
	
	UPROPERTY()
	FTransform Tansform;
};

USTRUCT()
struct RESONANCECORE_API FRTransformDataArray
{
	GENERATED_BODY()
public:
	
	UPROPERTY()
	TArray<FRPointData> TransformData;
};

USTRUCT()
struct RESONANCECORE_API FRBakeDataGroup
{
	GENERATED_BODY()
public:
	
	UPROPERTY()
	TMap<ERBakeType, FRTransformDataArray> BakeDataMap;
};

USTRUCT()
struct FRPathEdge 
{
	GENERATED_BODY()
public:
	UPROPERTY()
	int32 Index = INDEX_NONE;

	UPROPERTY()
	double Cost = 0.0;
	
	UPROPERTY()
	FVector CurrentForward = FVector::ForwardVector;
};

USTRUCT()
struct FRPathAdjacency
{
	GENERATED_BODY()
public:
	
	UPROPERTY()
	TArray<FRPathEdge> Edges;
};

USTRUCT()
struct FRGraph
{
	GENERATED_BODY()
public:
	
	UPROPERTY()
	TArray<FRPathAdjacency> AdjacencyList;
};