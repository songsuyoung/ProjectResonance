#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPatrolActor.generated.h"

class USplineComponent;

UCLASS()
class RESONANCE_API ARPatrolActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ARPatrolActor();
	
	USplineComponent* GetSplineComponent() { return SplineComponent; }
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline")
	TObjectPtr<USplineComponent> SplineComponent;
};
