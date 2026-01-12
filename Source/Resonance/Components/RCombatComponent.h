#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RCombatComponent.generated.h"

class ARWeaponBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RESONANCE_API URCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URCombatComponent();

	void Attack();

protected:
	virtual void BeginPlay() override;
	
	void OnWeaponAttack();
	void PlayAttackAnimation();
protected:

	/*
	* 프로토타입
	* TODO: 리팩토링 필요, 데이터 구조로 변경해야함. (데이터 매니저로 얻어올 수있도록)
	* 무기 변경이 언제든 가능한 구조로 유연하게 수정 예정 
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ARWeaponBase> WeaponClass;

protected:

	UPROPERTY(Transient)
	TObjectPtr<ARWeaponBase> Weapon;


};
