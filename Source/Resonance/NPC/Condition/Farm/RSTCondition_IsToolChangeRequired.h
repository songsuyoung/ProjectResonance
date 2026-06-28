#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "RSTCondition_IsToolChangeRequired.generated.h"

class ARNPCCharacter;

UCLASS()
class RESONANCE_API URSTCondition_IsToolChangeRequired : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()

public:
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<ARNPCCharacter> OwnerCharacter;
};
