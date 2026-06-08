#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Volume.h"
#include "RSpawnVolume.generated.h"

UCLASS()
class RESONANCE_API ARSpawnVolume : public AVolume
{
	GENERATED_BODY()

public:
	
	virtual void BeginPlay() override;
protected:
	
	UPROPERTY(EditAnywhere, Category = "Region|Settings")
	FName RegionID;
	
};
