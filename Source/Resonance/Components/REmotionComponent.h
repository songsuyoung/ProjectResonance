#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "REmotionComponent.generated.h"

/*
 * 목표 : EmotionStateMachine 을 관리하는 EmotionComponent
 * StatComponent를 약한 포인터로 가지고 있으며, 스탯으로 부터 Emotion 전환을 진행함.
 * Emotion에 의해 관리되어지는 모든 것을 가지고 있는 클래스
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RESONANCE_API UREmotionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UREmotionComponent();

	float GetStayDuration();
protected:
	virtual void BeginPlay() override;

		
};
