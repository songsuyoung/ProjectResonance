#include "Actor/RWayPointActor.h"

#include "Data/RCoreEnums.h"
#include "System/RBakeDataManager.h"

ARWayPointActor::ARWayPointActor()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = StaticMesh;
}

void ARWayPointActor::Interact(AActor* OtherActor)
{
	if (false == IsValid(OtherActor))
	{
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("[WayPoint] Find"));
	
	URBakeDataManager* WayPointManager = URBakeDataManager::Get(this);
	
	ensure(WayPointManager);
	
	FRTransformData WayPoint;
	bool bResult = WayPointManager->GetRandomTransformData(ERBakeType::WarpPoint, WayPoint);
	
	if (false == bResult)
	{
		// 데이터가 없어서 실패한 경우
		return;
	}
	
	OtherActor->TeleportTo(WayPoint.Tansform.GetLocation(), WayPoint.Tansform.GetRotation().Rotator());
}

FRTransformData ARWayPointActor::GetSerializedData() const
{
	return FRTransformData(ERPathPointType::Waypoint, GetActorTransform());
}

ERBakeType ARWayPointActor::GetBakeType() const
{
	return ERBakeType::WarpPoint;
}

void ARWayPointActor::BeginPlay()
{
	Super::BeginPlay();
	
}

