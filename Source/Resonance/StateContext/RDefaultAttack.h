#pragma once

#include "CoreMinimal.h"
#include "RStateContext.h"
#include "RDefaultAttack.generated.h"

class URCombatComponent;
class URSkillBase;

// 블루프린트화 시킬 수 있다.
UCLASS(Blueprintable)
class RESONANCE_API URDefaultAttack : public URStateContext
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(URActionStateComponent* InActionStateComponent)	override;
	virtual void Execute() override;
	
protected:
	
	UPROPERTY(Transient)
	TWeakObjectPtr<URCombatComponent> CombatComponent;
	
	UPROPERTY(Transient)
	TObjectPtr<URSkillBase> SkillObject;
protected:
	UPROPERTY(EditAnywhere, Category = "Skill")
	TSubclassOf<URSkillBase> SkillClass;
	
};
