#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "RSTCondition_IsExhausted.generated.h"

class AAIController;
class ARBaseCharacter;
class URBaseStatComponent;

UCLASS()
class RESONANCE_API URSTCondition_IsExhausted : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()

public:
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<AAIController> AIController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<ARBaseCharacter> OwnerCharacter;
	
protected:
	
	UPROPERTY(Transient)
	TWeakObjectPtr<URBaseStatComponent> StatComponent;
};
