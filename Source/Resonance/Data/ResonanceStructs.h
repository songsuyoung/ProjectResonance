#pragma once
#include "CoreMinimal.h"
#include "ResonanceStructs.generated.h"

USTRUCT()
struct FRPathRoutePayload
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Transient)
	int32 NearestPointIndex;
	
	UPROPERTY(Transient)
	int32 DestinationPointIndex;
	
};