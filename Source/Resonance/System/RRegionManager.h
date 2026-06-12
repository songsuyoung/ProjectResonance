#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RRegionManager.generated.h"

class ARRegionVolume;
enum class EREmotionState : uint8;

UCLASS(Blueprintable)
class RESONANCE_API URRegionManager : public UObject
{
	GENERATED_BODY()
	
public:
	static URRegionManager* Get(UObject* Context);
	
	void RegisterRegion(FName ID, ARRegionVolume* RegionVolume);
	bool FindRegionLocation(FName ID, FVector& OutLocation);
	TArray<FName> GetNextRegions(FName RegionID, EREmotionState NPCEmotionState);
	TWeakObjectPtr<ARRegionVolume> GetRegionVolume(FName RegionID);

protected:
	
	UPROPERTY(Transient)
	TMap<FName, TWeakObjectPtr<ARRegionVolume>> Regions;
};
