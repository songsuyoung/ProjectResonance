#include "RBaker.h"

#include "REditorUtils.h"
#include "Data/RCoreEnums.h"
#include "Data/RCoreStructs.h"

void FRTransformBaker::Bake(UWorld* World)
{
	// 버튼 한번 누르면, 엔진 상 월드에 있는 모든 IBakeable을 상속받은 액터들을 가져온다.
	// 액터들은, 각타입 , Transform을 저장하도록 한다.
	FRBakeDataGroup BakeDataGroup;
			
	TArray<IRBakeable*> WayPoints = FREditorUtils::CollectActorsWithInterface(World);
			
	for (IRBakeable* WayPoint : WayPoints)
	{
		FRTransformDataArray& TransformDataArray = BakeDataGroup.BakeDataMap.FindOrAdd(WayPoint->GetBakeType());
				
		TransformDataArray.TransformData.Add(WayPoint->GetSerializedData());
	}

	FREditorUtils::SerializeAndSave(BakeDataGroup, TEXT("/BakeData.json"));
}

void FRPathGraphBaker::Bake(UWorld* World, float LimitedDistance, float MaxSlopeTangent)
{
	TArray<IRBakeable*> Bakeables = FREditorUtils::CollectActorsWithInterface(World);
	
	TArray<FVector> Locations;
	for (IRBakeable* Bakeable : Bakeables)
	{
		if (ERBakeType::PathPoint != Bakeable->GetBakeType())
		{
			continue;
		}
		
		Locations.Add(Bakeable->GetSerializedData().Tansform.GetLocation());
	}
	
	MakeGraph(Locations, LimitedDistance, MaxSlopeTangent);
}

void FRPathGraphBaker::MakeGraph(const TArray<FVector>& Locations, float LimitedDistance, float MaxSlopeTangent)
{
	FRGraph GraphNode;
	GraphNode.AdjacencyList.SetNum(Locations.Num());
	
	UWorld* World = GEditor->GetEditorWorldContext().World();

	check(World);
	
	for (int32 RIndex = 0; RIndex < Locations.Num(); RIndex++)
	{
		for (int32 CIndex = RIndex + 1; CIndex < Locations.Num(); CIndex++)
		{
			// 띄운 이유 : PatrolActor가 바닥에 닿도록 End키를 누르기 때문에 항상 오브젝트와 부딪힐 수 있음으로, 임의의 높이만큼 올려준다.
			FVector StartLocation = Locations[RIndex] + FVector(0, 0, 10);
			FVector EndLocation = Locations[CIndex] + FVector(0, 0, 10);

			float Dist = FVector::Dist(StartLocation, EndLocation);
			if (Dist > LimitedDistance)
			{
				continue;
			}
			
			// tan계산을 통해서 각도 차이를 확인해 갈 수 있음과 없음을 체크한다.
			float Adjacent = FVector::Dist2D(StartLocation, EndLocation); // 수평거리
			float Height = FMath::Abs(EndLocation.Z - StartLocation.Z);   // 높이차

			float TanAngle = Height / Adjacent;

			if (TanAngle >= MaxSlopeTangent)
			{
				continue;
			}

			// LineTrace 장애물 존재 여부 확인
			FHitResult Hit;
			FCollisionQueryParams Params;
			
			bool bIsObstacle = World->LineTraceSingleByChannel
			(
				Hit,
				StartLocation,
				EndLocation,
				ECollisionChannel::ECC_WorldStatic,
				Params
				);
			
			if (bIsObstacle)
			{
				continue;
			}

			GraphNode.AdjacencyList[RIndex].Edges.Add({CIndex, Dist});
			GraphNode.AdjacencyList[RIndex].Edges.Add({RIndex, Dist});
			
			// 노드 생성 기준
			DrawDebugLine(World, StartLocation, EndLocation, FColor::Blue, false, 10.f, 0, 2.f);
		}
	}
	
	FREditorUtils::SerializeAndSave(GraphNode, TEXT("/GraphNodeData.json"));
}
