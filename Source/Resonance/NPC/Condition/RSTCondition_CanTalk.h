#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "RSTCondition_CanTalk.generated.h"

class AAIController;
class ARNPCCharacter;
class URBaseStatComponent;

UCLASS()
class RESONANCE_API URSTCondition_CanTalk : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()

public:
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<ARNPCCharacter> OwnerCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float SearchRange;
	
};
