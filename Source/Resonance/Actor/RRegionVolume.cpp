#include "Actor/RRegionVolume.h"

#include "Components/SplineComponent.h"
#include "System/RRegionManager.h"

ARRegionVolume::ARRegionVolume()
	: Super()
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("DirectionSplineComponent"));
	
	SplineComponent->SetupAttachment(RootComponent);
}

void ARRegionVolume::BeginPlay()
{
	Super::BeginPlay();

	URRegionManager* RegionManager = URRegionManager::Get(this);
	
	check(RegionManager);
	
	RegionManager->RegisterRegion(RegionID, this);
}

