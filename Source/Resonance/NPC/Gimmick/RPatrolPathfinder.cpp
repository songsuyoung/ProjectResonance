#include "NPC/Gimmick/RPatrolPathfinder.h"

// UE 5.
#include "EngineUtils.h"

// 
#include "Actor/RPatrolActor.h"

void URPatrolPathfinder::Init()
{
	if (false == Locations.IsEmpty())
	{
		return;
	}
	
	TArray<AActor*> PatrolPoints;
	// 배치된 PatrolActor 을 모두 가져온다.
	for (TActorIterator<ARPatrolActor> It(GetWorld()); It; ++It)
	{
		ARPatrolActor* Actor = *It;
		
		if (IsValid(Actor))
		{
			Locations.Add(Actor->GetActorLocation());
		}
	}
}

TArray<FVector> URPatrolPathfinder::FindPath(const FVector& StartLocation, const FVector& Destination)
{
	//Dijksta Algorithm
	TArray<FVector> OpenList;
	
	return OpenList;
}
