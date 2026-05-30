#pragma once
#include "CoreMinimal.h"
#include "ResonanceStructs.generated.h"

USTRUCT()
struct FRPatrolRoutePayload
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(Transient)
	FVector Destination;
};