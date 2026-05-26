#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "RAnimNotifyState_PlayNextCombo.generated.h"

class URCombatComponent;

UCLASS()
class RESONANCE_API URAnimNotifyState_PlayNextCombo : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:

	URAnimNotifyState_PlayNextCombo();
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	
	UPROPERTY(Transient)
	TWeakObjectPtr<URCombatComponent> CombatComponent;
};
