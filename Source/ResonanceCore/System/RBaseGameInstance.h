#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RBaseGameInstance.generated.h"

class URDataManager;
class URUIManagerBase;
class URNPCSpawnManager;
class URRegionManager;
class URBakeDataManager;
class UREventManager;
class URPathFinder;

UCLASS()
class RESONANCECORE_API URBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void BeginDestroy() override;
	
	virtual URDataManager* GetDataManager() { return nullptr; }
	virtual UREventManager* GetEventManager() { return nullptr; }
	
	// 정적으로 구워진 데이터
	virtual URBakeDataManager* GetBakeDataManager() { return nullptr; }
	virtual URPathFinder* GetPathFinder() { return nullptr; }	
	
	virtual URNPCSpawnManager* GetNPCSpawnManager() { return nullptr; }
	virtual URRegionManager* GetRegionManager() { return nullptr;}
};
