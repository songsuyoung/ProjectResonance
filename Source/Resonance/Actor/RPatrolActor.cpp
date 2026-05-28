#include "Actor/RPatrolActor.h"

// UE 5.
#include "Components/SplineComponent.h"

ARPatrolActor::ARPatrolActor()
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
	
	RootComponent = SplineComponent;
}

