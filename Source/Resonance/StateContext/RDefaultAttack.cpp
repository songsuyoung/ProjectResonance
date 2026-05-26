#include "RDefaultAttack.h"

#include "DrawDebugHelpers.h"    
#include "Character/ResonanceCharacter.h"
#include "Combat/Skill/RSkillBase.h"
#include "Components/RCombatComponent.h"
#include "Data/ResonanceEnums.h"

void URDefaultAttack::Initialize(URActionStateComponent* InActionStateComponent)
{
	Super::Initialize(InActionStateComponent);
	
	AResonanceCharacter* Character = Cast<AResonanceCharacter>(GetOuter());
	if (false == IsValid(Character))
	{
		return;
	}
	
	CombatComponent = Character->GetCombatComponent();
}

void URDefaultAttack::Execute()
{
	if (false == CombatComponent.IsValid())
	{
		return;
	}
	
	CombatComponent->Attack(ERSkillType::Default);
}
