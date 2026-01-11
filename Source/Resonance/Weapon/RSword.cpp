#include "Weapon/RSword.h"

// UE 5. 
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

// Newly Created Files...


ARSword::ARSword()
	: Super()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
}

void ARSword::PrepareAttack()
{
	Super::PrepareAttack();
}

void ARSword::ExecuteAttack()
{
	Super::ExecuteAttack();
}

void ARSword::EndAttack()
{
	Super::EndAttack();
}

void ARSword::ActivateWeapon()
{
	Super::ActivateWeapon();

}

void ARSword::DeactivateWeapon()
{
	Super::DeactivateWeapon();

}

void ARSword::UnEquip()
{
	Super::UnEquip();
}

void ARSword::AttachToSocket(EWeaponAttachLocation AttachLocation)
{
	Super::AttachToSocket(AttachLocation);

	ACharacter* OwnerCharacter = Cast<ACharacter>(Owner);

	if (IsValid(OwnerCharacter))
	{
		USkeletalMeshComponent* OwnerMeshComponent = OwnerCharacter->GetMesh();

		if (IsValid(OwnerMeshComponent))
		{
			const FVector& CharacterLocation = OwnerMeshComponent->GetSocketLocation(SocketName[AttachLocation]);

			const FName& Socket = WeaponSocketName.FindOrAdd(AttachLocation);

			const FVector& WeaponLocation = MeshComponent->GetSocketLocation(Socket);

			// 두 차이만큼 Offset 값을 이동시켜준다.
			const FVector& OffsetWorld = CharacterLocation - WeaponLocation;

			AddActorWorldOffset(OffsetWorld);
		}
	}
}
