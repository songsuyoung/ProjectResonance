#include "RAnimNotifyState_PlayNextCombo.h"

// UE
#include "DrawDebugHelpers.h"

// New Created Files...
#include "Character/ResonanceCharacter.h"
#include "Components/RCombatComponent.h"

URAnimNotifyState_PlayNextCombo::URAnimNotifyState_PlayNextCombo()
	: Super()
{
}

void URAnimNotifyState_PlayNextCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AResonanceCharacter* Character = Cast<AResonanceCharacter>(MeshComp->GetOwner());

	if (IsValid(Character))
	{
		CombatComponent = Character->GetCombatComponent();
		
		if (CombatComponent.IsValid())
		{
			CombatComponent->SetCanPlayNextCombo(true);
		}
	}
}

void URAnimNotifyState_PlayNextCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (CombatComponent.IsValid())
	{
		CombatComponent->SetCanPlayNextCombo(false);
	}
}
