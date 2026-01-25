#include "Animation/RAnimNotify_PlayNextCombo.h"

#include "Character/ResonanceCharacter.h"
#include "Components/RCombatComponent.h"

void URAnimNotify_PlayNextCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// 인터페이스로 빼야함.
	AResonanceCharacter* Character = Cast< AResonanceCharacter>(MeshComp->GetOwner());

	if (IsValid(Character))
	{
		URCombatComponent* CombatComponent = Character->GetCombatComponent();

		if (IsValid(CombatComponent))
		{
			CombatComponent->PlayNextCombo();
		}
	}
}