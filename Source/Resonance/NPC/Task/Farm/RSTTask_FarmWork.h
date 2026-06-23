#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "RSTTask_FarmWork.generated.h"

class AAIController;
class APawn;
class ARFarmPlotVolume;
enum class ERequiredFarmTask : uint8;

UCLASS()
class RESONANCE_API URSTTask_FarmWork : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	
protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<AAIController> AIController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<ACharacter> OwnerCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Plow|Settings")
	TObjectPtr<UAnimMontage> Animation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Plow|Settings")
	float WorkDuration;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Plow|Settings")
	ERequiredFarmTask FarmTask;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TWeakObjectPtr<ARFarmPlotVolume> FarmVolume;

protected:
	
	UPROPERTY(Transient)
	float ElapsedTime;
};
