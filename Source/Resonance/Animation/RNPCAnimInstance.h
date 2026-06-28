#pragma once

#include "CoreMinimal.h"
#include "RAnimInstance.h"
#include "Animation/AnimInstance.h"
#include "RNPCAnimInstance.generated.h"

class URJobComponent;
UCLASS()
class RESONANCE_API URNPCAnimInstance : public URAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	
	UPROPERTY(Transient)
	TWeakObjectPtr<URJobComponent> JobComponent;
	
	UPROPERTY(Transient, BlueprintReadOnly)
	uint8 bHasTool : 1;
};
