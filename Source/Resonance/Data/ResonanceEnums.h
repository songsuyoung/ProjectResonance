#pragma once
#include "CoreMinimal.h"

UENUM()
enum class ERDataTableType : uint8
{
	None,
	SkillData,
	CharacterData,
	Max,
};

ENUM_RANGE_BY_COUNT(ERDataTableType, ERDataTableType::Max)


UENUM()
enum class ERSkillType : uint8
{
	None,
	Default,			// 기본
	Heavy,				// 강공격
	ActiveE,			// ESkill
	ActiveQ,			// QSkill
};

UENUM()
enum class ERElementType : uint8
{
	None,
	Fire,
	Wind,
	Water,
};

UENUM()
enum class ERMessageType
{
	ShowUI,
};