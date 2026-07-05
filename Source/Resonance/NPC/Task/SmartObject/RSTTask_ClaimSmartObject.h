#pragma once

#include "CoreMinimal.h"
#include "SmartObjectRuntime.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "RSTTask_ClaimSmartObject.generated.h"

class ARNPCCharacter;
class AAIController;

UCLASS()
class RESONANCE_API URSTTask_ClaimSmartObject : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	
public:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<AAIController> AIController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<ARNPCCharacter> OwnerCharacter;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings")
	float SearchRadius;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Output")
	FSmartObjectClaimHandle ClaimHandle;
	
};
