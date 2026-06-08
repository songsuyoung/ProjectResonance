#pragma once
#include "CoreMinimal.h"

UENUM()
enum class ERBakeType : uint8
{
	WarpPoint,
	PathPoint,
};

UENUM()
enum class ERPathPointType : uint8
{
	Destination,
	Spawn,
	Waypoint,
};