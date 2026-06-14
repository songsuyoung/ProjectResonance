#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "RRegionVolume.generated.h"

class UArrowComponent;

UCLASS(Blueprintable)
class RESONANCE_API ARRegionVolume : public AVolume
{
	GENERATED_BODY()

public:
	ARRegionVolume();
	
	UArrowComponent* GetLookAtArrowComponent() { return LookAtArrowComponent; }
	
	// BeginPlay 함수에서는 NPC를 스폰한다.
	virtual void BeginPlay() override;
protected:
	
	UPROPERTY(EditAnywhere, Category = "Region|Settings")
	FName RegionID;
	
	UPROPERTY(EditInstanceOnly, Category = "Region|Settings")
	TObjectPtr<UArrowComponent> LookAtArrowComponent;
};
