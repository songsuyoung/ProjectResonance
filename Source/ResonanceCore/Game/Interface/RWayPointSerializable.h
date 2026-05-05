#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RWayPointSerializable.generated.h"

USTRUCT()
struct RESONANCECORE_API FRWayPoint
{
	GENERATED_BODY()
public:
	
	UPROPERTY()
	FVector Location;
	
	UPROPERTY()
	FRotator Rotation;
};

USTRUCT()
struct RESONANCECORE_API FRWayPointArray
{
	GENERATED_BODY()
public:
	
	UPROPERTY()
	TArray<FRWayPoint> WayPointData;
};

UINTERFACE()
class RESONANCECORE_API URWayPointSerializable : public UInterface
{
	GENERATED_BODY()
};

class RESONANCECORE_API IRWayPointSerializable
{
	GENERATED_BODY()

public:
	
	virtual FRWayPoint GetSerializedData() const = 0;
};
