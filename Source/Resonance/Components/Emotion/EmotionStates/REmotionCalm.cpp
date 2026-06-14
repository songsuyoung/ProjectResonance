#include "REmotionCalm.h"

UREmotionCalm::UREmotionCalm(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EmotionState = EREmotionState::Calm;
}

void UREmotionCalm::Enter()
{
	Super::Enter();
	
	UE_LOG(LogTemp, Warning, TEXT("Calm is Enter"));
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
