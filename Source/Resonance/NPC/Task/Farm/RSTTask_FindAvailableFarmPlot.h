#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "RSTTask_FindAvailableFarmPlot.generated.h"

class AAIController;
class ARFarmPlotVolume;
class ACharacter;

enum class ERequiredFarmTask : uint8;

UCLASS()
class RESONANCE_API URSTTask_FindAvailableFarmPlot : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	
public:
	
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<AAIController> AIController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<ACharacter> OwnerCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TWeakObjectPtr<ARFarmPlotVolume> FarmVolume;
};
