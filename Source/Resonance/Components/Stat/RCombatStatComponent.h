#pragma once

#include "CoreMinimal.h"
#include "RBaseStatComponent.h"
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
	
	/* BaseAttackPower : �⺻ ���ݷ� */
	UPROPERTY(EditInstanceOnly, Category = "Stat")
	float MaxBaseAttackPower;

	UPROPERTY(EditInstanceOnly, Category = "Stat")
	float CurrentBaseAttackPower;

	/* ���� �����͸� : ���� �����͸��� ���� ���� ����� �� �ִ� ���� ���� ����*/
	UPROPERTY(EditInstanceOnly, Category = "Stat")
	float BaseElementalMastery;

	/* 
	* ġ��Ÿ Ȯ�� : ġ��Ÿ Ȯ���� ���̴� ��� �ش� �ۼ��������� ���� ġ��Ÿ�� �޶��� 
	* Ȯ�� => �ۼ��������� ���
	*/
	UPROPERTY(EditInstanceOnly, Category = "Stat")
	float BaseHitChance;

	/* ���� ���� ȿ�� : ��ų ���� ȿ�� (�ۼ������� ���) */
	UPROPERTY(EditInstanceOnly, Category = "Stat")
	float EnerageRechargeRate;

};
