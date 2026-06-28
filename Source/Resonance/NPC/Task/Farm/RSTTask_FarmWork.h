#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "RSTTask_FarmWork.generated.h"

class AAIController;
class ARNPCCharacter;
class ARFarmPlotVolume;
class URJobComponent;
struct FRPlotData;
enum class ERequiredFarmTask : uint8;


UCLASS()
class RESONANCE_API URSTTask_FarmWork : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<AAIController> AIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<ARNPCCharacter> OwnerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Plow|Settings")
	TObjectPtr<UAnimMontage> WorkMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Plow|Settings")
	float MoveStepSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TWeakObjectPtr<ARFarmPlotVolume> FarmVolume;

protected:
	
	UPROPERTY(Transient)
	int32 ActiveCount;

	UPROPERTY(Transient)
	float Duration;

	UPROPERTY(Transient)
	float WorkDuration;

	UPROPERTY(Transient)
	float ElapsedTime;

	UPROPERTY(Transient)
	int32 ActivePlotIndex;

	UPROPERTY(Transient)
	FVector PlotCenter;

	UPROPERTY(Transient)
	bool NoMove;
};
