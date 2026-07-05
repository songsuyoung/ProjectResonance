#pragma once

#include "CoreMinimal.h"
#include "SmartObjectRuntime.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "RSTTask_ApproachAndAlignToSlot.generated.h"

class ARNPCCharacter;
class AAIController;
class USmartObjectSubsystem;

UCLASS()
class RESONANCE_API URSTTask_ApproachAndAlignToSlot : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	
public:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<AAIController> AIController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<ARNPCCharacter> OwnerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	FSmartObjectClaimHandle ClaimHandle;
	
protected:
	
	UPROPERTY(Transient)
	TWeakObjectPtr<USmartObjectSubsystem> SmartObjectSubsystem;
	
	UPROPERTY(Transient)
	FRotator TargetRotator;
	
	UPROPERTY(Transient)
	FVector TargetLocation;
};
