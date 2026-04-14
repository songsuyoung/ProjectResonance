#pragma once

#include "CoreMinimal.h"
#include "RWayPointManager.generated.h"

UCLASS()
class RESONANCE_API URWayPointManager : public UObject
{
	GENERATED_BODY()

public:
	static URWayPointManager* Get(UObject* Context);
	
	
};
