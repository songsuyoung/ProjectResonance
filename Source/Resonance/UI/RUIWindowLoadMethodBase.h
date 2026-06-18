#pragma once

#include "CoreMinimal.h"
#include "RUIWindowLoadMethodBase.generated.h"

UCLASS()
class RESONANCE_API URUIWindowLoadMethodBase : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintNativeEvent)
	UClass* LoadAssetClass(UClass* Class);
	
	virtual UClass* LoadAssetClass_Implementation(UClass* Class) { return nullptr; }
};
