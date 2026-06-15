#include "REmotionTired.h"

#include "GameFramework/CharacterMovementComponent.h"

UREmotionTired::UREmotionTired(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EmotionState = EREmotionState::Tired;
}

void UREmotionTired::Enter()
{
	Super::Enter();
	
	if (false == CharacterMovementComponent.IsValid())
	{
		return;
	}
	
	CharacterMovementComponent->MaxWalkSpeed =  BaseWalkSpeed * 0.6f;
}

void UREmotionTired::Tick()
{
	Super::Tick();
}

void UREmotionTired::Exit()
{
	Super::Exit();
	
	if (false == CharacterMovementComponent.IsValid())
	{
		return;
	}
	
	CharacterMovementComponent->MaxWalkSpeed =  BaseWalkSpeed;
}
