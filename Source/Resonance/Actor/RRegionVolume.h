#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "RRegionVolume.generated.h"

class USplineComponent;

UCLASS()
class RESONANCE_API ARRegionVolume : public AVolume
{
	GENERATED_BODY()

public:
	ARRegionVolume();
	
	USplineComponent* GetSplineComponent() { return SplineComponent; }
	// BeginPlay 함수에서는 NPC를 스폰한다.
	virtual void BeginPlay() override;
	
protected:
	
	UPROPERTY(EditAnywhere, Category = "Region|Settings")
	FName RegionID;
	
	// 앞으로 가야하는 방향
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Region|Settings")
	TObjectPtr<USplineComponent> SplineComponent;
	
};
