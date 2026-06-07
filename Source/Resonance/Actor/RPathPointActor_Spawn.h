#pragma once

#include "CoreMinimal.h"
#include "RPathPointActor.h"
#include "RPathPointActor_Spawn.generated.h"

// 월드에 갈 수 있는 경로에 배치될 액터
UCLASS()
class RESONANCE_API ARPathPointActor_Spawn : public ARPathPointActor
{
	GENERATED_BODY()
	
public:	
	ARPathPointActor_Spawn();
	
	virtual void BeginPlay() override;
	
protected:
	
	UPROPERTY(EditAnywhere, Category = "Settings")
	FName NPCID;
};
