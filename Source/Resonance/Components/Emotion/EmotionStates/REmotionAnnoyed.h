#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/Emotion/REmotionStateBase.h"
#include "REmotionAnnoyed.generated.h"

UCLASS(Blueprintable)
class RESONANCE_API UREmotionAnnoyed : public UREmotionStateBase
{
	GENERATED_BODY()

public:	
	UREmotionAnnoyed(const FObjectInitializer& ObjectInitializer);

public:
	virtual void Enter() override;
	virtual void Tick() override;
	virtual void Exit()	override;
};
