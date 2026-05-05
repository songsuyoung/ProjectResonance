#pragma once

#include "CoreMinimal.h"
#include "Game/Interface/RWayPointSerializable.h"
#include "GameFramework/Actor.h"
#include "Interface/RInteractable.h"
#include "RWayPointActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class RESONANCE_API ARWayPointActor : public AActor, public IRInteractable, public IRWayPointSerializable
{
	GENERATED_BODY()
	
public:	
	ARWayPointActor();

	virtual void Interact(AActor* OtherActor);
	
	virtual FRWayPoint GetSerializedData() const;
	
protected:
	virtual void BeginPlay() override;

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	
};
