#pragma once

#include "CoreMinimal.h"
#include "RStateContext.h"
#include "RDefaultAttack.generated.h"

class URCombatComponent;

// 블루프린트화 시킬 수 있다.
UCLASS(Blueprintable)
class RESONANCE_API URDefaultAttack : public URStateContext
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(AActor* InOwner) override;
	
	virtual void Execute() override;
	
protected:
	
	UPROPERTY(Transient)
	TWeakObjectPtr<URCombatComponent> CombatComponent;
	
};
