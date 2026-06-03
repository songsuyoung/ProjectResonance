#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"


// 기존 Transform 저장 (일반화)
class FRTransformBaker
{
public:
	static void Bake(UWorld* World);
};

struct FRTransformDataArray;

// PathGraph 전용 (구체화)
class FRPathGraphBaker
{
public:
	static void Bake(UWorld* World, FRTransformDataArray& PathDataArray, float LimitedDistance, float MaxSlopeTangent);
	
protected:
	
	static void MakeGraph(const TArray<FVector>& Locations, float LimitedDistance, float MaxSlopeTangent);
};