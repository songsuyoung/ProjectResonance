#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "RSTTask_FindCircuitPath.generated.h"

class ARBaseCharacter;
class AAIController;

UCLASS()
class RESONANCE_API URSTTask_FindCircuitPath : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:
	URSTTask_FindCircuitPath(const FObjectInitializer& ObjectInitializer);
	
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<AAIController> AIController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<APawn> OwnerCharacter;
};
