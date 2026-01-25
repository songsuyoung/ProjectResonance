#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RGameInstance.generated.h"

class URDataManager;
class URUIManagerBase;
class UREventManager;

UCLASS()
class RESONANCE_API URGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	URDataManager* GetDataManager() { return DataManager; }
	UREventManager* GetEventManager() { return EventManager; }
protected:

	UPROPERTY(EditAnywhere, Category = "AssetClass")
	TSubclassOf<URDataManager> DataManagerClass;

protected:

	UPROPERTY(Transient)
	TObjectPtr<URDataManager> DataManager;

	UPROPERTY(Transient)
	TObjectPtr<UREventManager> EventManager;

	UPROPERTY(Transient)
	TObjectPtr<URUIManagerBase> UIManager;

};
