#pragma once

#include "CoreMinimal.h"
#include "Game/Interface/RBakeable.h"
#include "GameFramework/Actor.h"
#include "RPathPointActor.generated.h"

// 월드에 갈 수 있는 경로에 배치될 액터
UCLASS()
class RESONANCE_API ARPathPointActor : public AActor, public IRBakeable
{
	GENERATED_BODY()
	
public:	
	ARPathPointActor();

	virtual FRTransformData GetSerializedData() const override;
	virtual ERBakeType GetBakeType() const;
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	ERPathPointType PathPointType;
	
};
