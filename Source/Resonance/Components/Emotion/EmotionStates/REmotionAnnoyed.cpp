#include "REmotionAnnoyed.h"

#include "GameFramework/CharacterMovementComponent.h"

UREmotionAnnoyed::UREmotionAnnoyed(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EmotionState = EREmotionState::Annoyed;
}

void UREmotionAnnoyed::Enter()
{
	Super::Enter();
	
	if (false == CharacterMovementComponent.IsValid())
	{
		return;
	}
	
	CharacterMovementComponent->MaxWalkSpeed =  BaseWalkSpeed * 1.1f;
}

void UREmotionAnnoyed::Tick()
{
	Super::Tick();
}

void UREmotionAnnoyed::Exit()
{
	Super::Exit();
	
		
	if (false == CharacterMovementComponent.IsValid())
	{
		return;
	}
	
	CharacterMovementComponent->MaxWalkSpeed =  BaseWalkSpeed;
}
