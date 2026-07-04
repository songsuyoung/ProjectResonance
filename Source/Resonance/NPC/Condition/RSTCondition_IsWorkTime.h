#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "RSTCondition_IsWorkTime.generated.h"

class AAIController;
class ARBaseCharacter;
class URBaseStatComponent;

UCLASS()
class RESONANCE_API URSTCondition_IsWorkTime : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()

public:
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<AAIController> AIController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<ARBaseCharacter> OwnerCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	int32 StartHour;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	int32 EndHour;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bInvert : 1;
};
