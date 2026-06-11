#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RAIController.generated.h"
class UStateTreeAIComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnRegionVisited, FName);
UCLASS()
class RESONANCE_API ARAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ARAIController();
	
	FOnRegionVisited OnRegionVisited;
	
protected:
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	
	void VisitRegion(FName RegionID);
	
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStateTreeAIComponent> StateTreeComponent;
};
