#include "REmotionAngry.h"

UREmotionAngry::UREmotionAngry(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EmotionState = EREmotionState::Angry;
}

void UREmotionAngry::Enter()
{
	Super::Enter();
	
	UE_LOG(LogTemp, Warning, TEXT("Angry is Enter"));
}

void UREmotionAngry::Tick()
{
	Super::Tick();
}

void UREmotionAngry::Exit()
{
	Super::Exit();
	
	UE_LOG(LogTemp, Warning, TEXT("Angry is Exit"));
}
