#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RHitCheckComponent.generated.h"

struct FRWeaponSocketInfo;
struct FOverlapResult;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RESONANCE_API URHitCheckComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	URHitCheckComponent();

	void ProcessAttackHit(const TArray<FOverlapResult>& InTarget);

protected:

	void InternalProcessHitCheck(const TArray<FOverlapResult>& InTarget);

protected:

	// 누적을 위한 위치 Offset
	UPROPERTY(Transient)
	TArray<FVector> AccumulatedOffset;
};
