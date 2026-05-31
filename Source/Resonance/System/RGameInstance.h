#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RGameInstance.generated.h"

class URDataManager;
class URUIManagerBase;
class URWayPointManager;
class UREventManager;
class URPathFinder;

UCLASS()
class RESONANCE_API URGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	URDataManager* GetDataManager() { return DataManager; }
	UREventManager* GetEventManager() { return EventManager; }
	URWayPointManager* GetWayPointManager() { return WayPointManager; }
	URPathFinder* GetPathFinder() { return PathFinder; }	
protected:

	UPROPERTY(EditAnywhere, Category = "AssetClass")
	TSubclassOf<URDataManager> DataManagerClass;

protected:

	UPROPERTY(Transient)
	TObjectPtr<URPathFinder> PathFinder;
	
	UPROPERTY(Transient)
	TObjectPtr<URDataManager> DataManager;

	UPROPERTY(Transient)
	TObjectPtr<UREventManager> EventManager;

	UPROPERTY(Transient)
	TObjectPtr<URWayPointManager> WayPointManager;
	
	UPROPERTY(Transient)
	TObjectPtr<URUIManagerBase> UIManager;

};
