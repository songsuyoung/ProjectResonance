#pragma once
#include "CoreMinimal.h"

UENUM()
enum class ERBakeType : uint8
{
	WarpPoint,
	PathPoint,
};


UENUM(BlueprintType)
enum class ERPathPointType : uint8
{
	Spawn,       // NPC 스폰 지점
	Destination, // 목적지
	Waypoint,    // 경유지
	Walk,        // 걷는 구간
	Run,         // 뛰는 구간
	RightPathOn, // 우측 경로 시작
	RightPathOff, // 우측 경로 끝 -> 원래 길찾기 복귀
};