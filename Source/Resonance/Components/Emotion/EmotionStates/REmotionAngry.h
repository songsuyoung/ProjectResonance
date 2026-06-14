#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/Emotion/REmotionStateBase.h"
#include "REmotionAngry.generated.h"

UCLASS(Blueprintable)
class RESONANCE_API UREmotionAngry : public UREmotionStateBase
{
	GENERATED_BODY()

public:	
	UREmotionAngry(const FObjectInitializer& ObjectInitializer);

public:
	virtual void Enter() override;
	virtual void Tick() override;
	virtual void Exit()	override;
};
