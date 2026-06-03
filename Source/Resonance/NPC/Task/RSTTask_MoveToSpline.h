#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "RSTTask_MoveToSpline.generated.h"

class ARNPCCharacter;
class AAIController;
class USplineComponent;

struct FSplinePoint;
UCLASS()
class RESONANCE_API URSTTask_MoveToSpline : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:
	URSTTask_MoveToSpline(const FObjectInitializer& ObjectInitializer);
	
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition);

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

protected:
	
	void Move(float DeltaTime);
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<AAIController> AIController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<ARNPCCharacter> OwnerCharacter;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<USplineComponent> SplineComponent;
	
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
