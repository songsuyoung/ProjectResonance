#include "Actor/RPatrolActor_Line.h"

// UE 5.
#include "Components/SplineComponent.h"

ARPatrolActor_Line::ARPatrolActor_Line()
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
	
	RootComponent = SplineComponent;
}

