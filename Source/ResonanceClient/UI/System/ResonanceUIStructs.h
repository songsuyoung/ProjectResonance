#pragma once
#include "CoreMinimal.h"
#include "Data/RCommonEnums.h"
#include "ResonanceUIStructs.generated.h"

USTRUCT(BlueprintType)
struct FRUIStatInfo
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, Category = "Stat |Settings")
	ERStatType StatType;
	
	UPROPERTY(EditAnywhere, Category = "Stat |Settings")
	float Value;
};