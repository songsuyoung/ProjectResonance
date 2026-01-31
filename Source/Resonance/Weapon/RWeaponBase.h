#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RWeaponBase.generated.h"

class UMeshComponent;
class URAttackAction;

UENUM()
enum class ERWeaponAttachLocation : uint8
{
	None,
	Hand,		/* 손에 지고 있는 것 */
	Holster,	/*휴대하면서 필요할 때 빠르고 쉽게 꺼낼 수 있도록 만드는 것*/
};

UENUM()
enum class ERWeaponActiveState : uint8
{
	Inactive,	/*비활성화*/
	Active		/*활성화*/
};

UENUM()
enum class ERWeaponEquipState : uint8
{
	Unequipped,	/*미장착중*/
	Equipped,	/*장착중*/
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

	bool IsUnsable();
	bool IsWeaponActive();
	bool IsEquipped();

	void GetHitCheckSocketLocation(TMap<FName, FVector>& OutSocketInfo);

public:
	virtual void ActivateWeapon();
	virtual void DeactivateWeapon();
	virtual void UnEquip();

protected:
	virtual void BeginPlay() override;
	virtual void AttachToSocket(ERWeaponAttachLocation AttachLocation);

protected:

	/* 공통 변수 선언 */
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UMeshComponent> MeshComponent;

	// 무기의 기본 공격력
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat")
	float BaseAttackDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat")
	TMap<ERWeaponAttachLocation, FName> SocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitCheck")
	TArray<FName> HitCheckSocketName;

protected:

	UPROPERTY(Transient)
	ERWeaponActiveState ActiveState;

	UPROPERTY(Transient)
	ERWeaponEquipState EquipState;
};
