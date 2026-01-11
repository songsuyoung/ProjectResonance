#pragma once

#include "CoreMinimal.h"
#include "Stat/RBaseStatComponent.h"
#include "RCombatStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RESONANCE_API URCombatStatComponent : public URBaseStatComponent
{
	GENERATED_BODY()

public:	
	
	URCombatStatComponent();

protected:
	
	virtual void BeginPlay() override;

protected:
	
	/* BaseAttackPower : 기본 공격력 */
	UPROPERTY(EditInstanceOnly, Category = "Stat")
	float MaxBaseAttackPower;

	UPROPERTY(EditInstanceOnly, Category = "Stat")
	float CurrentBaseAttackPower;

	/* 원소 마스터리 : 원소 마스터리가 높을 수록 사용할 수 있는 원소 힘이 강력*/
	UPROPERTY(EditInstanceOnly, Category = "Stat")
	float BaseElementalMastery;

	/* 
	* 치명타 확률 : 치명타 확률을 높이는 방법 해당 퍼센테이지에 따라 치명타가 달라짐 
	* 확률 => 퍼센테이지에 기반
	*/
	UPROPERTY(EditInstanceOnly, Category = "Stat")
	float BaseHitChance;

	/* 원소 충전 효율 : 스킬 충전 효율 (퍼센테이지 기반) */
	UPROPERTY(EditInstanceOnly, Category = "Stat")
	float EnerageRechargeRate;

};
