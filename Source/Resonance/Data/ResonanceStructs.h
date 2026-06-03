#pragma once
#include "CoreMinimal.h"
#include "ResonanceStructs.generated.h"

USTRUCT()
struct FRPathRoutePayload
{
	GENERATED_BODY()
	
public:
	FRPathRoutePayload() { }
	FRPathRoutePayload(const TArray<FVector>& Locations) : PathLocation(Locations) {}

	UPROPERTY(Transient)
	TArray<FVector> PathLocation;
};