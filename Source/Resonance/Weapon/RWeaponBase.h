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
	Hand,		/* �տ� ���� �ִ� �� */
	Holster,	/*�޴��ϸ鼭 �ʿ��� �� ������ ���� ���� �� �ֵ��� ����� ��*/
};

UENUM()
enum class ERWeaponActiveState : uint8
{
	Inactive,	/*��Ȱ��ȭ*/
	Active		/*Ȱ��ȭ*/
};

UENUM()
enum class ERWeaponEquipState : uint8
{
	Unequipped,	/*��������*/
	Equipped,	/*������*/
};

// ���丮 �޼ҵ� ���� ����
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
	void GetHitCheckSocketLocation(TArray<FVector>& OutSocketInfo);
	FName GetSocketName(ERWeaponAttachLocation AttachLocation) { return SocketName[AttachLocation]; }
public:
	virtual void ActivateWeapon();
	virtual void DeactivateWeapon();
	virtual void UnEquip();

protected:
	virtual void BeginPlay() override;
	virtual void AttachToSocket(ERWeaponAttachLocation AttachLocation);

protected:

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UMeshComponent> MeshComponent;

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
