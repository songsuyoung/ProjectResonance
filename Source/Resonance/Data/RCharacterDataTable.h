#pragma once

#include "CoreMinimal.h"
#include "RBaseDataTable.h"
#include "Data/ResonanceEnums.h"
#include "RCharacterDataTable.generated.h"

USTRUCT(BlueprintType)
struct FRComboSkill
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, Category = "SkillBase")
	int32 SkillOrder;
	
	UPROPERTY(EditDefaultsOnly, Category = "SkillBase")
	FName SkillID;
};

USTRUCT(BlueprintType)
struct FRComboSkillContainer
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, Category = "SkillBase")
	TArray<FRComboSkill> SkillContainer;
};

USTRUCT(BlueprintType)
struct RESONANCE_API FRCharacterDataTable : public FRBaseDataTable
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "CharacterBase")
	ERElementType ElementType;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterBase")
	TMap<ERSkillType, FRComboSkillContainer> SkillContainer;
};
