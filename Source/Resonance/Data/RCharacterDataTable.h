#pragma once

#include "CoreMinimal.h"
#include "Data/ResonanceEnums.h"
#include "RCharacterDataTable.generated.h"

USTRUCT(BlueprintType)
struct FRComboSkillContainer
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "SkillBase")
	TArray<FName> SkillIDs;
};

USTRUCT(BlueprintType)
struct RESONANCE_API FRCharacterDataTable : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "CharacterBase")
	ERElementType ElementType;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterBase")
	TMap<ERSkillType, FRComboSkillContainer> SkillContainer;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterBase|Input")
	TMap<ERSkillType, float> InputHoldTime;
};
