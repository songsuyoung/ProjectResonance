#pragma once
#include "CoreMinimal.h"

UENUM()
enum class ERBakeType : uint8
{
	WarpPoint,
	PathPoint,
};

UENUM()
enum class ERPathPointType : uint8
{
	Destination,
	Spawn,
	Waypoint,
};

UENUM()
enum class ERStatType : uint8
{
	HP,
	Stamina,
	Mood,
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
	Max,
};

ENUM_RANGE_BY_COUNT(EREmotionState, EREmotionState::Max)
