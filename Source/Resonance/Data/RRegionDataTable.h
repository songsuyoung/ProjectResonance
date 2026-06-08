#pragma once

#include "CoreMinimal.h"
#include "RRegionDataTable.generated.h"

USTRUCT(BlueprintType)
struct RESONANCE_API FRRegionDataTable : public FTableRowBase
{
	GENERATED_BODY()
public:
	// 장소 이름
	UPROPERTY(EditDefaultsOnly, Category = "Region|Settings")
	FString RegionName;

	// 장소 위치 (스폰/목표 지점 위치)
	UPROPERTY(EditDefaultsOnly, Category = "Region|Settings")
	FVector Location;
};
