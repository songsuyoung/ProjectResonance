#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "RSTTask_SelectNextNode_RightHandRule.generated.h"

class ARBaseCharacter;
class AAIController;

USTRUCT()
struct FRRouteComparator
{
	GENERATED_BODY()
public:
	
	bool operator()(const TPair<int32, double> RouteA, const TPair<int32, double> RouteB) const
	{
		return RouteA.Value < RouteB.Value;
	}
};

UCLASS()
class RESONANCE_API URSTTask_SelectNextNode_RightHandRule : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:
	URSTTask_SelectNextNode_RightHandRule(const FObjectInitializer& ObjectInitializer);
	
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition);

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	float NearbyRadius;
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<AAIController> AIController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<APawn> OwnerCharacter;
};
