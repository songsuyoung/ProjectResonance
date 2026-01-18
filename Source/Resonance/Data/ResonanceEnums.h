#pragma once
#include "CoreMinimal.h"

UENUM()
enum class ERDataTableType : uint8
{
	None,
	SkillData,
	Max,
};

ENUM_RANGE_BY_COUNT(ERDataTableType, ERDataTableType::Max)


UENUM()
enum class ERSkillType : uint8
{
	Default,		// ±âº»
	ESkill,			// ESkill
	QSkill,			// QSkill
};
