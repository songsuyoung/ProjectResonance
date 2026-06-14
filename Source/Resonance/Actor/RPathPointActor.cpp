#include "Actor/RPathPointActor.h"

#include "Data/RCommonEnums.h"
#include "Data/RCommonStructs.h"

ARPathPointActor::ARPathPointActor()
	: Super()
	, PathPointType(ERPathPointType::Waypoint)
{

}

FRPointData ARPathPointActor::GetSerializedData() const
{
	return FRPointData(PathPointType, GetTransform());
}

ERBakeType ARPathPointActor::GetBakeType() const
{
	return ERBakeType::PathPoint;
}
