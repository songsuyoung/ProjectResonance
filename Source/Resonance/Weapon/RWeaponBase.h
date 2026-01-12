#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RWeaponBase.generated.h"

class UMeshComponent;
class URAttackAction;

UENUM()
enum class EWeaponAttachLocation : uint8
{
	None,
	Hand,		/* 손에 지고 있는 것 */
	Holster,	/*휴대하면서 필요할 때 빠르고 쉽게 꺼낼 수 있도록 만드는 것*/
};

// 팩토리 메소드 패턴 응용
UCLASS(Abstract)
class RESONANCE_API ARWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ARWeaponBase();
	
	//GET/SET
	FORCEINLINE UMeshComponent* GetMeshComponent() { return MeshComponent; }
	
	// TODO: 어떤 스킬을 실행하는지 전달하도록 함.
	DECLARE_MULTICAST_DELEGATE(FOnWeaponAttack)
	FOnWeaponAttack OnWeaponAttack;

public:
	bool TryAttack();
	// 나중에는 무기에서 선택된 Action을 리턴하도록 한다.
	URAttackAction* GetAttackAction() { return DefaultAction; }
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:

	// 공격 가능한가 여부 확인
	bool Attack();

	/*
	* 현재는 공격을 Weapon에 두었지만, 실제 공격은 하나의 스킬이 되어질 예정
	* 즉, 그말은 일반 공격/E 스킬/Q 스킬이 달라질 예정으로, 스킬에 대한 오브젝트화
	* 시킬 예정이기 때문에 쿨타임이 존재하는가? 여부를 확인할 필요가 있음
	*/
	virtual void PrepareAttack() PURE_VIRTUAL(PrepareAttack, );
	virtual void EndAttack() PURE_VIRTUAL(EndAttack, );

	virtual bool CanAttack() { return true; }
	virtual void ExecuteAttack();
	virtual void ActivateWeapon();
	virtual void DeactivateWeapon();
	virtual void UnEquip();

	virtual void AttachToSocket(EWeaponAttachLocation AttachLocation);
protected:

	/* 공통 변수 선언 */
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UMeshComponent> MeshComponent;
	
	/* [프로토 타입]
	* TODO : 세 변수 모두 데이터화 시켜야 한다.
	*/

	// 무기를 장착하고 나서, 활성화되어지는 시간
	// 공격이 없으면, 무기를 넣는다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat")
	float ActivationDelay;

	// 무기를 넣고나서, 무기가 비활성화 되어지는 시간
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat")
	float DeactivationDelay;

	// 무기의 기본 공격력
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat")
	float BaseAttackDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat")
	TMap<EWeaponAttachLocation, FName> SocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action")
	TObjectPtr<URAttackAction> DefaultAction;

protected:

	// 무기의 Visibility 관련 변수
	UPROPERTY(Transient)
	uint8 bIsWeaponActive : 1;

	UPROPERTY(Transient)
	uint8 bIsEquipped : 1;

	// 마지막 공격 이후에 공격하지 않은 채로 경과한 시간
	// 만약, 경과한 시간이 ActivationDelay 보다 클 경우, 무기를 넣을 예정
	UPROPERTY(Transient)
	float TimeSinceLastAttack;

	// 무기를 집어넣고나서, 그 이후에 경과 시간
	UPROPERTY(Transient)
	float TimeSinceWeaponDeactivated;

	// 마지막으로 공격한 시간
	UPROPERTY(Transient)
	float LastAttackTime;

	UPROPERTY(Transient)
	float LastUnequipTime;

};
