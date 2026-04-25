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

// Skill Type 도 콤보로 나눈다.
UENUM()
enum class ERSkillType : uint8
{
	None,
	Default,			// 없앨예정 -> 지금 오류가 너무많음. (스킬시스템 구현을 위해 잠시)
	DefaultA,			// 기본 콤보 A->B->C->D..
	DefaultB,
	DefaultC,
	DefaultD,
	Heavy,				// 강공격
	Plunge,				// 낙공
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

UENUM()
enum class ERInputContext : uint8
{
	Attack,
	Hold,
	SkillE,
	SkillQ
};

UENUM()
enum class ERActionContext : uint8
{
	None,
	// 캐릭터 행동 관련
	Action_Move,
	Action_Run_Pressed,
	Action_Run_Released,
	Action_Jump,
	Action_Interact,
	
	// 스킬 관련
	Action_Default_0,
	Action_Default_1,
	Action_Default_2,
	Action_Default_3,
};
