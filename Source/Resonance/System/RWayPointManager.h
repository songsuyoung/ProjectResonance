#pragma once

#include "CoreMinimal.h"
#include "RWayPointManager.generated.h"

struct FRWayPoint;

UCLASS()
class RESONANCE_API URWayPointManager : public UObject
{
	GENERATED_BODY()

public:
	static URWayPointManager* Get(UObject* Context);
	const FRWayPoint& GetRandomPoint() const;
	
	void Initialize();
	
protected:
	
	TArray<FRWayPoint> WayPoints;
	
};
