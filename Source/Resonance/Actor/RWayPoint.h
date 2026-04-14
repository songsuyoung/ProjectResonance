#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/RInteractable.h"
#include "RWayPoint.generated.h"

class UStaticMeshComponent;
UCLASS()
class RESONANCE_API ARWayPoint : public AActor, public IRInteractable
{
	GENERATED_BODY()
	
public:	
	ARWayPoint();

	virtual void Interact(AActor* OtherActor);
	
protected:
	virtual void BeginPlay() override;

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	
};
