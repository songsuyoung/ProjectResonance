#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CollisionQueryParams.h"
#include "RAnimNotifyState_HitCheck.generated.h"

class URHitCheckComponent;
class ARWeaponBase;

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
	
	bool TryHitCheck(const FVector& StartLocation, const FVector& TargetLocation);
protected:

	UPROPERTY(Transient)
	TWeakObjectPtr<URHitCheckComponent> HitCheckComponent;

	UPROPERTY(Transient)
	TWeakObjectPtr<ARWeaponBase> Weapon;
	
	UPROPERTY(Transient)
	TWeakObjectPtr<UWorld> World;
	
	UPROPERTY(Transient)
	TArray<FHitResult> Target;

	FCollisionQueryParams Params;
	
	FTransform PrevBoneTransform;

protected:

	UPROPERTY(EditAnywhere, Category = "Resonance|DebugDraw")
	uint8 bDebugDraw : 1;

	UPROPERTY(EditAnywhere, Category = "Resonance|CollisionLength")
	float CollisionHalfLength;
	
	UPROPERTY(EditAnywhere, Category = "Resonance|Dist")
	float DiffDist;
};
