#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "RSTTask_DecideNextLocation.generated.h"

class ARNPCCharacter;
class AAIController;

UCLASS()
class RESONANCE_API URSTTask_DecideNextLocation : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:
	URSTTask_DecideNextLocation(const FObjectInitializer& ObjectInitializer);
	
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<AAIController> AIController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<ARNPCCharacter> OwnerCharacter;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	FName NextRegionID;
};
