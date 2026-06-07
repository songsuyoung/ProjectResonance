#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RGameInstance.generated.h"

class URDataManager;
class URUIManagerBase;
class URNPCSpawnManager;
class URBakeDataManager;
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
	
	// 정적으로 구워진 데이터
	URBakeDataManager* GetBakeDataManager() { return BakeDataManager; }
	URPathFinder* GetPathFinder() { return PathFinder; }	
	
	URNPCSpawnManager* GetNPCSpawnManager() { return NPCSpawnManager; }
	
protected:
	
protected:

	UPROPERTY(EditAnywhere, Category = "AssetClass")
	TSubclassOf<URDataManager> DataManagerClass;

	UPROPERTY(EditAnywhere, Category = "AssetClass")
	TSubclassOf<URNPCSpawnManager> NPCSpawnManagerClass;

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

};
