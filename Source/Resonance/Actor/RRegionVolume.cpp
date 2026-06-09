#include "Actor/RRegionVolume.h"

#include "Character/RNPCCharacter.h"
#include "System/RNPCSpawnManager.h"
#include "System/RRegionManager.h"

ARRegionVolume::ARRegionVolume()
	: Super()
{
	OnActorBeginOverlap.AddDynamic(this, &ThisClass::OnActorOverlapBegin);
}

void ARRegionVolume::BeginPlay()
{
	Super::BeginPlay();

	URRegionManager* RegionManager = URRegionManager::Get(this);
	
	check(RegionManager);
	
	RegionManager->RegisterRegion(RegionID, this);
}

void ARRegionVolume::OnActorOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	ARNPCCharacter* NPCCharacter = Cast<ARNPCCharacter>(OverlappedActor);
	
	if (IsValid(NPCCharacter))
	{
		NPCCharacter->VisitRegion(RegionID);
	}
}
