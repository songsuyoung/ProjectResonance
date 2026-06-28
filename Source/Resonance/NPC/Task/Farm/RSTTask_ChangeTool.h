#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "RSTTask_ChangeTool.generated.h"

class AAIController;
class ARNPCCharacter;
class URJobComponent;

UCLASS()
class RESONANCE_API URSTTask_ChangeTool : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<AAIController> AIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<ARNPCCharacter> OwnerCharacter;

protected:
	
	UPROPERTY(Transient)
	TWeakObjectPtr<URJobComponent> JobComponent;
};
