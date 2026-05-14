#include "RDefaultAttack.h"

#include "DrawDebugHelpers.h"    
#include "Character/ResonanceCharacter.h"
#include "Combat/Skill/RSkillBase.h"
#include "Components/RCombatComponent.h"

void URDefaultAttack::Initialize(URActionStateComponent* InActionStateComponent)
{
	Super::Initialize(InActionStateComponent);
	
	AResonanceCharacter* Character = Cast<AResonanceCharacter>(GetOuter());
	if (false == IsValid(Character))
	{
		return;
	}
	
	CombatComponent = Character->GetCombatComponent();
	
	if (false == CombatComponent.IsValid())
	{
		return;
	}
	
	SkillObject = NewObject<URSkillBase>(Character, SkillClass);
	SkillObject->Init(Character);
	
	SkillObject->OnCooldownEventDelegate.AddUObject(CombatComponent.Get(), &URCombatComponent::OnCooldownEventDelegate);
	SkillObject->OnAttackCompleted.AddUObject(CombatComponent.Get(), &URCombatComponent::OnAttackCompleted);
	SkillObject->OnAttackCompleted.AddUObject(this, &ThisClass::OnExit);
	SkillObject->OnAttackStarted.AddUObject(CombatComponent.Get(), &URCombatComponent::OnAttackStarted);
}

void URDefaultAttack::Execute()
{
	if (false == CombatComponent.IsValid())
	{
		return;
	}
	
	CombatComponent->Attack(SkillObject);
	
	UE_LOG(LogTemp, Log, TEXT("[Skill|Object] %s"), *SkillObject->GetName());
}
