#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RGameInstance.generated.h"

class URDataManager;
class URUIManagerBase;

UCLASS()
class RESONANCE_API URGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	URDataManager* GetDataManager() { return DataManager; }

protected:

	UPROPERTY(EditAnywhere, Category = "AssetClass")
	TSubclassOf<URDataManager> DataManagerClass;

protected:

	UPROPERTY(Transient)
	TObjectPtr<URDataManager> DataManager;

	UPROPERTY(Transient)
	TObjectPtr<URUIManagerBase> UIManager;
};
