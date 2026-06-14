#include "REmotionAnnoyed.h"

UREmotionAnnoyed::UREmotionAnnoyed(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EmotionState = EREmotionState::Annoyed;
}

void UREmotionAnnoyed::Enter()
{
	Super::Enter();
	
	UE_LOG(LogTemp, Warning, TEXT("Annoyed is Enter"));
}

void UREmotionAnnoyed::Tick()
{
	Super::Tick();
}

void UREmotionAnnoyed::Exit()
{
	Super::Exit();
	
	UE_LOG(LogTemp, Warning, TEXT("Annoyed is Exit"));
}
