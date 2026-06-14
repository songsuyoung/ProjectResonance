#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "RSTTask_EnterRegion.generated.h"

class ARNPCCharacter;
class AAIController;

UENUM()
enum class EREnterRegionStage : uint8
{
	TurnInPlace,
	Walk,
	Enter,
	Hidden
};

UCLASS()
class RESONANCE_API URSTTask_EnterRegion : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:
	URSTTask_EnterRegion(const FObjectInitializer& ObjectInitializer);
	
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition);
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

protected:
	
	void TurnInPlace(float DeltaTime);
	void MoveTo(float DeltaTime);
	void Enter();
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<AAIController> AIController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<ARNPCCharacter> OwnerCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float SlowDownRadius;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float LimitedDistanceScale;
protected:
	
	UPROPERTY(Transient)
	EREnterRegionStage EnterRegion;
	
	UPROPERTY(Transient)
	FRotator TargetRotation;
	
	UPROPERTY(Transient)
	FVector TargetLocation;
	
	UPROPERTY(Transient)
	float LimitedDistance;
	
	UPROPERTY(Transient)
	FName NextRegionID;
};
