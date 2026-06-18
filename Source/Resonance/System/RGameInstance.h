#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "System/RBaseGameInstance.h"
#include "RGameInstance.generated.h"

class URDataManager;
class URUIManagerBase;
class URNPCSpawnManager;
class URRegionManager;
class URBakeDataManager;
class UREventManager;
class URPathFinder;

UCLASS()
class RESONANCE_API URGameInstance : public URBaseGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void BeginDestroy() override;
	
	virtual URDataManager* GetDataManager() override { return DataManager; }
	virtual UREventManager* GetEventManager() override { return EventManager; }
	
	// 정적으로 구워진 데이터
	virtual URBakeDataManager* GetBakeDataManager() override { return BakeDataManager; }
	virtual URPathFinder* GetPathFinder() override { return PathFinder; }	
	
	virtual URNPCSpawnManager* GetNPCSpawnManager() override { return NPCSpawnManager; }
	virtual URRegionManager* GetRegionManager() override { return RegionManager;}
	
	virtual URUIManagerBase* GetUIManager() { return UIManager; }

protected:

	UPROPERTY(EditAnywhere, Category = "AssetClass")
	TSubclassOf<URDataManager> DataManagerClass;

	UPROPERTY(EditAnywhere, Category = "AssetClass")
	TSubclassOf<URNPCSpawnManager> NPCSpawnManagerClass;

	UPROPERTY(EditAnywhere, Category = "AssetClass")
	TSubclassOf<URUIManagerBase> UIManagerClass;

protected:

	UPROPERTY(Transient)
	TObjectPtr<URPathFinder> PathFinder;
	
	UPROPERTY(Transient)
	TObjectPtr<URDataManager> DataManager;

	UPROPERTY(Transient)
	TObjectPtr<UREventManager> EventManager;
	
	UPROPERTY(Transient)
	TObjectPtr<URNPCSpawnManager> NPCSpawnManager;

	UPROPERTY(Transient)
	TObjectPtr<URBakeDataManager> BakeDataManager;
	
	UPROPERTY(Transient)
	TObjectPtr<URUIManagerBase> UIManager;
	
	UPROPERTY(Transient)
	TObjectPtr<URRegionManager> RegionManager;
};
