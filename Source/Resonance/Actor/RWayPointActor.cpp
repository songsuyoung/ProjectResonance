#include "Actor/RWayPointActor.h"

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
}

FRWayPoint ARWayPointActor::GetSerializedData() const
{
	FRWayPoint WayPoint(GetActorLocation(), GetActorRotation());
	
	return WayPoint;
}

void ARWayPointActor::BeginPlay()
{
	Super::BeginPlay();
	
}

