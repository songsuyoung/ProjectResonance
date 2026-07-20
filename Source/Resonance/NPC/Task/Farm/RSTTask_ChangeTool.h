#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "RSTTask_ChangeTool.generated.h"

class AAIController;
class ARNPCCharacter;
class URJobComponent;
enum class ERToolType : uint8;
UCLASS()
class RESONANCE_API URSTTask_ChangeTool : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<AAIController> AIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Context")
	TWeakObjectPtr<ARNPCCharacter> OwnerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	uint8 bClear : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ERToolType ToolType;

protected:
	
	UPROPERTY(Transient)
	TWeakObjectPtr<URJobComponent> JobComponent;
};
