#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RAIController.generated.h"
class UStateTreeAIComponent;

UCLASS()
class RESONANCE_API ARAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ARAIController();
	
protected:
	
	virtual void OnPossess(APawn* InPawn) override;
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStateTreeAIComponent> StateTreeComponent;
};
