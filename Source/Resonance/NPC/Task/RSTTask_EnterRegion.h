#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "RSTTask_EnterRegion.generated.h"

UENUM()
enum class EREnterRegionStage : uint8
{
	TurnInPlace,
	Walk,
	Hidden
};

class ARRegionVolume;
class AAIController;
class ARNPCCharacter;
UCLASS()
class RESONANCE_API URSTTask_EnterRegion : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:
	URSTTask_EnterRegion(const FObjectInitializer& ObjectInitializer);
	
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

protected:
	
	void TurnInPlace(float DeltaTime);
	void Walk(float DeltaTime);
	void Enter();
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<AAIController> AIController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<ARNPCCharacter> OwnerCharacter;
	
protected:
	UPROPERTY(Transient)
	EREnterRegionStage EnterRegion;
	
	UPROPERTY(Transient)
	FRotator TargetRotation;
	
	UPROPERTY(Transient)
	FVector TargetLocation;
	
	UPROPERTY(Transient)
	float LimitedDistance;
};
