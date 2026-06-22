#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "RSTTask_NavigateToPlot.generated.h"

class AAIController;
class APawn;
class ARFarmPlotVolume;
class URAnimInstance;
struct FAIRequestID;
namespace EPathFollowingResult { enum Type : int; }

enum class ERequiredFarmTask : uint8;
UCLASS()
class RESONANCE_API URSTTask_NavigateToPlot : public UStateTreeTaskBlueprintBase
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
	
	// NPC에게 맞는 FarmVolume을 상위 State에서 찾아 가지고있고,
	// In을 통해서 연결해 사용한다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TWeakObjectPtr<ARFarmPlotVolume> FarmVolume;

protected:
	
	UPROPERTY(Transient)
	TWeakObjectPtr<URAnimInstance> AnimInstance;
	
	UPROPERTY(Transient)
	TArray<FVector> PathPoints;
	
	UPROPERTY(Transient)
	int32 PathIndex;	
};
