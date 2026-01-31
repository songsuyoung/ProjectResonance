#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RHitCheckComponent.generated.h"

struct FRWeaponSocketInfo;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RESONANCE_API URHitCheckComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	URHitCheckComponent();

	void ProcessAttackHit(AActor* Weapon);

	UFUNCTION()
	void HandleMontageStarted(UAnimMontage* Montage);
protected:

	virtual void BeginPlay() override;

	void InternalProcessHitCheck(TSet<AActor*> OutOverlapActor, const TMap<FName, FVector>& CurrentOffset);

protected:


	// 누적을 위한 위치 Offset
	UPROPERTY(Transient)
	TMap<FName, FVector> AccumulatedOffset;
};
