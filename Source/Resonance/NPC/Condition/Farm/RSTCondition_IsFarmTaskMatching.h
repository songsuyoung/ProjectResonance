#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "RSTCondition_IsFarmTaskMatching.generated.h"

enum class ERequiredFarmTask : uint8;

UCLASS()
class RESONANCE_API URSTCondition_IsFarmTaskMatching : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()
	
public:
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	ERequiredFarmTask PendingFarmTask;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FarmTask|Settings")
	TArray<ERequiredFarmTask> ExpectedFarmTasks;
};
