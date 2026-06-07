#pragma once

#include "CoreMinimal.h"
#include "Game/Interface/RBakeable.h"
#include "GameFramework/Actor.h"
#include "Interface/RInteractable.h"
#include "RWayPointActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class RESONANCE_API ARWayPointActor : public AActor, public IRInteractable, public IRBakeable
{
	GENERATED_BODY()
	
public:	
	ARWayPointActor();

	virtual void Interact(AActor* OtherActor);
	
	virtual FRPointData GetSerializedData() const override;
	virtual ERBakeType GetBakeType() const override;
	
protected:
	virtual void BeginPlay() override;

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	
};
