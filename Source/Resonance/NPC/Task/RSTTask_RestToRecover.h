#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "RSTTask_RestToRecover.generated.h"

class ARBaseCharacter;
class URBaseStatComponent;

UCLASS()
class RESONANCE_API URSTTask_RestToRecover : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:

	URSTTask_RestToRecover(const FObjectInitializer& ObjectInitializer);
	
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<ARBaseCharacter> OwnerCharacter;
	
protected:
	
	UPROPERTY(EditAnywhere, Category = "In")
	float RecoveryRate;
	
	UPROPERTY(EditAnywhere, Category = "In")
	float TargetPercentage;
	
protected:
	
	UPROPERTY(Transient)
	TWeakObjectPtr<URBaseStatComponent> StatComponent;
	
	UPROPERTY(Transient)
	float TargetStamaina;
	
};
