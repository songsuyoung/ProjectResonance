#include "Actor/RPathPointActor.h"

#include "Data/RCoreEnums.h"
#include "Data/RCoreStructs.h"

ARPathPointActor::ARPathPointActor()
	: Super()
	, PathPointType(ERPathPointType::Waypoint)
{

}

FRTransformData ARPathPointActor::GetSerializedData() const
{
	return FRTransformData(PathPointType, GetTransform());
}

ERBakeType ARPathPointActor::GetBakeType() const
{
	return ERBakeType::PathPoint;
}
