#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "RAnimNotifyState_HitCheck.generated.h"

class URHitCheckComponent;
class ARWeaponBase;
struct FOverlapResult;

UCLASS()
class RESONANCE_API URAnimNotifyState_HitCheck : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	URAnimNotifyState_HitCheck();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:

	UPROPERTY(Transient)
	TWeakObjectPtr<URHitCheckComponent> HitCheckComponent;

	UPROPERTY(Transient)
	TWeakObjectPtr<ARWeaponBase> Weapon;

	UPROPERTY(Transient)
	TArray<FVector> PrePoint;

	UPROPERTY(Transient)
	TArray<FOverlapResult> Target;

protected:

	UPROPERTY(EditAnywhere, Category = "Resonance|DebugDraw")
	uint8 bDebugDraw : 1;

	UPROPERTY(EditAnywhere, Category = "Resonance|DebugDraw")
	float FixedFrame;

};
