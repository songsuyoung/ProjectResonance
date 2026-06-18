#pragma once

#include "CoreMinimal.h"
#include "UI/RUIWindowLoadMethodBase.h"
#include "RUIWindowLoadMethod.generated.h"

class URCommonActivatableWidget;

UCLASS(Blueprintable)
class RESONANCECLIENT_API URUIWindowLoadMethod : public URUIWindowLoadMethodBase
{
	GENERATED_BODY()
	
public:
	
	virtual UClass* LoadAssetClass_Implementation(UClass* Class) override;
	
protected:
	
	UPROPERTY(EditAnywhere, Category = "Window | UI")
	TMap<FName, TSoftClassPtr<URCommonActivatableWidget>> WindowMap;
};
