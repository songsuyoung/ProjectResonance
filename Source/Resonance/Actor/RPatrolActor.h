#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPatrolActor.generated.h"

UENUM(BlueprintType)
enum class ERPatrolPointType : uint8
{
	Spawn,       // NPC 스폰 지점
	Destination, // 목적지
	Waypoint,    // 경유지
	Walk,        // 걷는 구간
	Run,         // 뛰는 구간
};

// 월드에 갈 수 있는 경로에 배치될 액터
UCLASS()
class RESONANCE_API ARPatrolActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ARPatrolActor();

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	ERPatrolPointType PatrolPointType;
	
};
