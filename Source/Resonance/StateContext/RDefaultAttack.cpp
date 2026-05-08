#include "RDefaultAttack.h"

#include "DrawDebugHelpers.h"    
#include "Character/ResonanceCharacter.h"
#include "Components/RCombatComponent.h"

void URDefaultAttack::Initialize(AActor* InOwner)
{
	AResonanceCharacter* Character = Cast<AResonanceCharacter>(InOwner);
	
	if (IsValid(Character))
	{
		CombatComponent = Character->GetCombatComponent();
	}
}

void URDefaultAttack::Execute()
{
	if (false == CombatComponent.IsValid())
	{
		return;
	}
	
	CombatComponent->RequestTransition(Condition.CurrentTags);
}
