#pragma once

#include "CoreMinimal.h"
#include "RBaseDataTable.h"
#include "RNPCDataTable.generated.h"

USTRUCT(BlueprintType)
struct RESONANCE_API FRNPCDataTable : public FRBaseDataTable
{
	GENERATED_BODY()

public:
	
	// TODO: NPC마다 이름, 외형 등도 여기에 있을 예정
	// NPC들 마다 첫 시작 스탯도 정의될 예정
	UPROPERTY(EditDefaultsOnly, Category = "NPCBase")
	float SpawnTime; // StartTime + SpawnTime 이후에 스폰될 예정

	// TODO: NPCCharacter에 BaseDuration 반영예정.
	UPROPERTY(EditDefaultsOnly, Category = "NPCBase")
	float BaseDuration;
	
	UPROPERTY(EditDefaultsOnly, Category = "NPCBase")
	FName HomeID;
	
	// ID, 퍼센테이지
	UPROPERTY(EditDefaultsOnly, Category = "NPCBase")
	TMap<FName, float> PreferredLocations;
	
	// ID, 퍼센테이지
	UPROPERTY(EditDefaultsOnly, Category = "NPCBase")
	TMap<FName, float> DislikedRegions;
};
