#include "Weapon/RSword.h"

// UE 5. 
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

// Newly Created Files...


ARSword::ARSword()
	: Super()
{
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
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

void ARSword::AttachToSocket(ERWeaponAttachLocation AttachLocation)
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

			// �� ���̸�ŭ Offset ���� �̵������ش�.
			const FVector& OffsetWorld = CharacterLocation - WeaponLocation;

			AddActorWorldOffset(OffsetWorld);
		}
	}
}
