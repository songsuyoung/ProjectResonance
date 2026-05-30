#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RPatrolPathfinder.generated.h"

UCLASS()
class RESONANCE_API URPatrolPathfinder : public UObject
{
	GENERATED_BODY()

public:
	// ARPatrolActor을 모두 가져오고 캐싱한다.
	void Init();	
	
	// 현재 캐릭터의 위치 -> 목적지 까지의 위치를 담은 벡터 전달
	TArray<FVector> FindPath(const FVector& StartLocation, const FVector& Destination);

protected:
	
	// Point 지점들
	UPROPERTY()
	TArray<FVector> Locations;
};
