#pragma once

#include "CoreMinimal.h"
#include "Game/Interface/RBakeable.h"
#include "GameFramework/Actor.h"
#include "RPathPointActor.generated.h"

UENUM(BlueprintType)
enum class ERPathPointType : uint8
{
	Spawn,       // NPC 스폰 지점
	Destination, // 목적지
	Waypoint,    // 경유지
	Walk,        // 걷는 구간
	Run,         // 뛰는 구간
};

// 월드에 갈 수 있는 경로에 배치될 액터
UCLASS()
class RESONANCE_API ARPathPointActor : public AActor, public IRBakeable
{
	GENERATED_BODY()
	
public:	
	ARPathPointActor();

	ERPathPointType GetPathPointType() { return PathPointType; }
	virtual FRTransformData GetSerializedData() const;
	virtual ERBakeType GetBakeType() const;
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	ERPathPointType PathPointType;
	
};
