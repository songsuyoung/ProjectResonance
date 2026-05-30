#pragma once

#include "CoreMinimal.h"
#include "RPatrolActor.h"
#include "GameFramework/Actor.h"
#include "RPatrolActor_Line.generated.h"

class USplineComponent;

// spline component로 경로 표현할 때 사용
UCLASS()
class RESONANCE_API ARPatrolActor_Line : public ARPatrolActor
{
	GENERATED_BODY()
	
public:	
	ARPatrolActor_Line();
	
	USplineComponent* GetSplineComponent() { return SplineComponent; }
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline")
	TObjectPtr<USplineComponent> SplineComponent;
};
