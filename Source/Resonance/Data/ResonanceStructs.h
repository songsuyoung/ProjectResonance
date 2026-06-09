#pragma once
#include "CoreMinimal.h"
#include "ResonanceStructs.generated.h"

USTRUCT()
struct FRPathRoutePayload
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(Transient)
	FName NextRegionID;
	
	UPROPERTY(Transient)
	TArray<FVector> PathLocation;
};