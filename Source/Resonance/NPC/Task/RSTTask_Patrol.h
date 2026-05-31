#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Navigation/PathFollowingComponent.h"
#include "RSTTask_Patrol.generated.h"

class ARBaseCharacter;
class AAIController;
class USplineComponent;
struct FSplinePoint;

using namespace EPathFollowingResult;

UCLASS()
class RESONANCE_API URSTTask_Patrol : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:
	URSTTask_Patrol(const FObjectInitializer& ObjectInitializer);
	
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition);

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<AAIController> AIController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<APawn> OwnerCharacter;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USplineComponent> SplineComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float AcceptableRadius;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float SlowDownRadius;
	
protected:
	UPROPERTY(Transient)
	TArray<FSplinePoint> SplinePoints;
	
	UPROPERTY(Transient)
	int32 CurrentSplineIndex;
};
