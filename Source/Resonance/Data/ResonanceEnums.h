#pragma once
#include "CoreMinimal.h"

UENUM()
enum class ERDataTableType : uint8
{
	None,
	SkillData,
	CharacterData,
	NPCDataTable,
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
	Action_Attack,
	
	// 스킬 관련
	Action_Default_0,
	Action_Default_1,
	Action_Default_2,
	Action_Default_3,
};

// Enum값을 우선 순위로 배치한다. 
// 내림차순으로, 0번이 가장 높은 우선순위를 차지한다.
// 현재 구현 우선 순위 : Angry/Tired/Calm/Annoyed 를 가장 먼저 구현한다.
UENUM()
enum class EREmotionState : uint8
{
	Angry = 0,	 /* 화가 난 상태 */
	Sad,		 /* 슬픈 상태 */
	Tired,		 /* 피곤한 상태 */
	Annoyed,	 /* 짜증난 상태 */
	Happy,		 /* 행복한 상태 */
	Calm,		 /* 차분한 상태 */
};

UENUM(BlueprintType)
enum class ERConditionValueType : uint8
{
	Value      UMETA(DisplayName = "Value"),
	Percentage UMETA(DisplayName = "Percentage"),
};

UENUM()
enum class ERArithmeticOperation : uint8
{
	Greater,
	GreaterOrEqual,
	Less,
	LessOrEqual,
	Equal,
	NotEqual,
};
