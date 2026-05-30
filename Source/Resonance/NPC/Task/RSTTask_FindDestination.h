#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "RSTTask_FindDestination.generated.h"
class AAIController;
class APawn;

UCLASS()
class RESONANCE_API URSTTask_FindDestination : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	
public:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<AAIController> AIController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<APawn> OwnerCharacter;
	
};
