#include "Actor/RWayPoint.h"

ARWayPoint::ARWayPoint()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = StaticMesh;
}

void ARWayPoint::Interact(AActor* OtherActor)
{
	if (false == IsValid(OtherActor))
	{
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("[WayPoint] Find"));
}

void ARWayPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

