#include "Components/REmotionComponent.h"

UREmotionComponent::UREmotionComponent()
{

}

void UREmotionComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

float UREmotionComponent::GetStayDuration()
{
	// 추후에 현재 Emotion이 무엇인가에 따라서 머무는 시간이 달라짐.	
	
	//0~1 사이의 값을 가짐 
	float DurationTime = FMath::FRand() * FMath::RandRange(10.f, 50.f);
	
	return DurationTime;
}
