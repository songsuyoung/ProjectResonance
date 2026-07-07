#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "SmartObject/RSTTask_ClaimSmartObject.h"
#include "RSTTask_PlayAnimMontage.generated.h"

class ARNPCCharacter;
class AAIController;

UCLASS()
class RESONANCE_API URSTTask_PlayAnimMontage : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	
public:
	URSTTask_PlayAnimMontage(const FObjectInitializer& ObjectInitializer);
	
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	UFUNCTION()
	void OnAnimMontageBlendOut(UAnimMontage* AnimMontage, bool bInterrupted);
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<ARNPCCharacter> OwnerCharacter;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<AActor> SmartObjectActor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	FSmartObjectClaimHandle ClaimHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimMontage> AnimationMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FName MotionWarpingName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	uint8 bUseMotionWarping : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	uint8 bWaitForAnimationToFinish :1;
};
