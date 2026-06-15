#include "REmotionAngry.h"

#include "Components/Stat/RBaseStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UREmotionAngry::UREmotionAngry(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EmotionState = EREmotionState::Angry;
}

void UREmotionAngry::Enter()
{
	Super::Enter();
	
	if (false == CharacterMovementComponent.IsValid())
	{
		return;
	}
	
	CharacterMovementComponent->MaxWalkSpeed =  BaseWalkSpeed * 2.0f;
}

void UREmotionAngry::Tick()
{
	Super::Tick();
}

void UREmotionAngry::Exit()
{
	Super::Exit();
	
	CharacterMovementComponent->MaxWalkSpeed = BaseWalkSpeed;
}
