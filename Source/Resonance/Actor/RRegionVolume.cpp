#include "Actor/RRegionVolume.h"

#include "Character/RNPCCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/SplineComponent.h"
#include "System/RRegionManager.h"

ARRegionVolume::ARRegionVolume()
	: Super()
{
	LookAtArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("LookAtArrowComponent"));
	LookAtArrowComponent->SetupAttachment(RootComponent);
}

void ARRegionVolume::BeginPlay()
{
	Super::BeginPlay();

	URRegionManager* RegionManager = URRegionManager::Get(this);
	
	check(RegionManager);
	
	RegionManager->RegisterRegion(RegionID, this);
}
