#pragma once
#include "CoreMinimal.h"
#include "Data/ResonanceEnums.h"
#include "RSkillDataTable.generated.h"

class URSkillBase;

USTRUCT(BlueprintType)
struct FRSkillDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	ERSkillType SkillType;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TSubclassOf<URSkillBase> SkillClass;
};