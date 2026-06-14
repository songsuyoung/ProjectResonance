#include "REmotionTired.h"

UREmotionTired::UREmotionTired(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	EmotionState = EREmotionState::Tired;
}

void UREmotionTired::Enter()
{
	Super::Enter();
	
	UE_LOG(LogTemp, Warning, TEXT("Tired is Enter"));
}

void UREmotionTired::Tick()
{
	Super::Tick();
}

void UREmotionTired::Exit()
{
	Super::Exit();
	
	UE_LOG(LogTemp, Warning, TEXT("Tired is Exit"));
}
