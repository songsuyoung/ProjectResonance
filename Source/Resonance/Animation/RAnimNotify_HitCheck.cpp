
#include "Animation/RAnimNotify_HitCheck.h"

// Newly Created Files
#include "Character/ResonanceCharacter.h"
#include "Components/RHitCheckComponent.h"
#include "Components/RCombatComponent.h"
#include "Weapon/RWeaponBase.h"

void URAnimNotify_HitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// 실제 애님 노티파이를 실행하는 SkeletalMeshComponent 
	// GetOwner -> Character
	// Character-> HitCheckComponent -> ProcessAttackHit 호출
	// 추후에 공통으로 빼기 위해서, CombatComponent, HitCheckComponent StatComponent 등 을 하나로 묶은 인터페이스가 필요하다.
	// ProcessAttackHit 함수에서 이들을 처리하는 코드를 한번에 호출해야한다.
	// 이유 : 나중에 AI 또한 이 코드를 사용할 수 있기 때문이다.
	if (false == IsValid(MeshComp))
	{
		return;
	}

	AResonanceCharacter* Character = Cast<AResonanceCharacter>(MeshComp->GetOwner());
	
	if (IsValid(Character))
	{
		URCombatComponent* CombatComponent = Character->GetCombatComponent();

		AActor* Weapon = nullptr;
		if (IsValid(CombatComponent))
		{
			Weapon = CombatComponent->GetWeapon();
		}
		// 인터페이스에서 ProcessHitCheck 함수를 호출하면 처리할 수 있도록 수정해야한다.
		URHitCheckComponent* HitCheckComponent = Character->GetHitCheckComponent();

		if (IsValid(HitCheckComponent))
		{
			HitCheckComponent->ProcessAttackHit(Weapon);
		}
	}
}
