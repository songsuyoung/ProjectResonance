#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RRegionManager.generated.h"

class ARRegionVolume;

UCLASS(Blueprintable)
class RESONANCE_API URRegionManager : public UObject
{
	GENERATED_BODY()
	
public:
	static URRegionManager* Get(UObject* Context);
	
	void RegisterRegion(FName ID, ARRegionVolume* RegionVolume);
	bool FindRegionLocation(FName ID, FVector& OutLocation);
	

protected:
	
	UPROPERTY(Transient)
	TMap<FName, TWeakObjectPtr<ARRegionVolume>> Regions;
};
