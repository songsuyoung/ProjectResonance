#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Navigation/PathFollowingComponent.h"
#include "RSTTask_Patrol.generated.h"

class ARBaseCharacter;
class AAIController;
class USplineComponent;

using namespace EPathFollowingResult;

UCLASS()
class RESONANCE_API URSTTask_Patrol : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition);
	
	void ReceiveMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);
protected:
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Context)
	TWeakObjectPtr<ARBaseCharacter> OwnerCharacter;
	
	UPROPERTY(Transient, BlueprintReadOnly, Category = Context)
	TWeakObjectPtr<AAIController> AIController;
	
	UPROPERTY(Transient, BlueprintReadOnly)
	TWeakObjectPtr<USplineComponent> SplineComponent;
	
	UPROPERTY(Transient, BlueprintReadOnly)
	int32 SplinePointIndex;
	
	UPROPERTY(Transient, BlueprintReadOnly)
	int32 LastSplinePointIndex;
};
