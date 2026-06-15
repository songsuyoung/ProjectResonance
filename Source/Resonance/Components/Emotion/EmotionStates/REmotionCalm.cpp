#include "REmotionCalm.h"

#include "GameFramework/CharacterMovementComponent.h"

UREmotionCalm::UREmotionCalm(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EmotionState = EREmotionState::Calm;
}

void UREmotionCalm::Enter()
{
	Super::Enter();
	
	if (false == CharacterMovementComponent.IsValid())
	{
		return;
	}
	
	CharacterMovementComponent->MaxWalkSpeed =  BaseWalkSpeed ;
}

void UREmotionCalm::Tick()
{
	Super::Tick();
}

void UREmotionCalm::Exit()
{
	Super::Exit();
	
	UE_LOG(LogTemp, Warning, TEXT("Calm is Exit"));
}
