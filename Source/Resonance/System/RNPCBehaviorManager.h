#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RNPCBehaviorManager.generated.h"

class URActionData;

UCLASS()
class RESONANCE_API URNPCBehaviorManager : public UObject
{
	GENERATED_BODY()
	
public:
	FName GetRandomAction();
	UAnimMontage* FindAction(const FName& ActionName);
	bool CheckAction(const FName& VerbActionName, const FName& NounActionName);
protected:
	
	UPROPERTY(Transient)
	TMap<FName, TObjectPtr<URActionData>> VerbActionData;
	
	UPROPERTY(Transient)
	TMap<FName, TObjectPtr<URActionData>> NounActionData;
};
