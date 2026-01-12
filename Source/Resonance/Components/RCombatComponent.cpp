#include "RCombatComponent.h"

// UE 5.
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

// Newly Created File 
#include "Data/Action/RAttackAction.h"
#include "Weapon/RWeaponBase.h"

URCombatComponent::URCombatComponent()
{
}

void URCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	check(World);

	FActorSpawnParameters Params;

	// URCombatComponent 의 Owner값이 Actor의 Owner로 지정
	Params.Owner = GetOwner();
	// 마켓에서 가져온 메시를 어떻게 처리할지 확인해야한다.
	Weapon = World->SpawnActor<ARWeaponBase>(WeaponClass, Params);


	if (IsValid(Weapon))
	{
		Weapon->OnWeaponAttack.AddUObject(this, &ThisClass::OnWeaponAttack);
	}
}

void URCombatComponent::OnWeaponAttack()
{
	PlayAttackAnimation();
}

void URCombatComponent::PlayAttackAnimation()
{
	if (false == IsValid(Weapon))
	{
		return;
	}

	URAttackAction* Action = Weapon->GetAttackAction();

	if (false == IsValid(Action))
	{
		return;
	}

	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (IsValid(Character))
	{
		Character->PlayAnimMontage(Action->AnimMontage);
	}
}

void URCombatComponent::Attack()
{
	if (IsValid(Weapon))
	{
		bool bAttackSucceeded = Weapon->TryAttack();

		// 실패할 때의 처리를 할 것인가?
	}
}