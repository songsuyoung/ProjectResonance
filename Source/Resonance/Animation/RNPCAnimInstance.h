#pragma once

#include "CoreMinimal.h"
#include "RAnimInstance.h"
#include "Animation/AnimInstance.h"
#include "RNPCAnimInstance.generated.h"

UCLASS()
class RESONANCE_API URNPCAnimInstance : public URAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

};
