#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "RSTTask_Talk.generated.h"

class ARNPCCharacter;
class AAIController;

UCLASS()
class RESONANCE_API URSTTask_Talk : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:
	URSTTask_Talk(const FObjectInitializer& ObjectInitializer);
	
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<AAIController> AIController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<ARNPCCharacter> OwnerCharacter;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Output")
	TWeakObjectPtr<ACharacter> TargetCharacter;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Output")
	uint8 bTalk : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float SearchRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TObjectPtr<UAnimMontage> AnimMotage;
};
