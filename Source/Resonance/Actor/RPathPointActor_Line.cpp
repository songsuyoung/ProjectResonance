#include "Actor/RPathPointActor_Line.h"

// UE 5.
#include "Components/SplineComponent.h"

ARPathPointActor_Line::ARPathPointActor_Line()
{
	SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
	
	RootComponent = SplineComponent;
}

