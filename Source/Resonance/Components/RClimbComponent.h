
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RClimbComponent.generated.h"

class ACharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RESONANCE_API URClimbComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URClimbComponent(const FObjectInitializer& ObjectInitializer);
	
	bool RequestClimb();
	
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	void StartClimb();
	bool CanClimb();

	// 나중에 구조를 분리한다.
	bool ComputeClimbVerticalBounds(TSet<int32>& HitResults, const FCollisionQueryParams& QueryParams,const FVector& StartLocation, float Distance);
	bool ComputeClimbSideBounds(TSet<int32>& HitResults, const FCollisionQueryParams& QueryParams,const FVector& StartLocation, float Distance);

protected:
	UPROPERTY(Transient)
	uint8 bIsClimbing : 1;
	
	UPROPERTY(Transient)
	TWeakObjectPtr<ACharacter> OwnerPawn;
	
		
};
