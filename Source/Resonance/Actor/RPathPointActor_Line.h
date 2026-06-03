#pragma once

#include "CoreMinimal.h"
#include "RPathPointActor.h"
#include "GameFramework/Actor.h"
#include "RPathPointActor_Line.generated.h"

class USplineComponent;

// spline component로 경로 표현할 때 사용
UCLASS()
class RESONANCE_API ARPathPointActor_Line : public ARPathPointActor
{
	GENERATED_BODY()
	
public:	
	ARPathPointActor_Line();
	
	USplineComponent* GetSplineComponent() { return SplineComponent; }
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline")
	TObjectPtr<USplineComponent> SplineComponent;
};
