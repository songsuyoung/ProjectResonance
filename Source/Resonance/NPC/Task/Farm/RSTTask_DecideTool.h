#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "RSTTask_DecideTool.generated.h"

class AAIController;
class ARNPCCharacter;
class ARFarmPlotVolume;

UCLASS()
class RESONANCE_API URSTTask_DecideTool : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<AAIController> AIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<ARNPCCharacter> OwnerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TWeakObjectPtr<ARFarmPlotVolume> FarmVolume;
};
