#pragma once

#include "CoreMinimal.h"
#include "Weapon/RWeaponBase.h"
#include "RSword.generated.h"


UCLASS()
class RESONANCE_API ARSword : public ARWeaponBase
{
	GENERATED_BODY()
	
public:
	ARSword();

protected:
	virtual void ActivateWeapon() override;
	virtual void DeactivateWeapon() override;
	virtual void UnEquip() override;
	virtual void AttachToSocket(EWeaponAttachLocation AttachLocation) override;
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stat")
	TMap<EWeaponAttachLocation, FName> WeaponSocketName;

};
