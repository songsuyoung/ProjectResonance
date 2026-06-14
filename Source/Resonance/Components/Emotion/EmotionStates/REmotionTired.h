#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/Emotion/REmotionStateBase.h"
#include "REmotionTired.generated.h"

UCLASS(Blueprintable)
class RESONANCE_API UREmotionTired : public UREmotionStateBase
{
	GENERATED_BODY()

public:	
	UREmotionTired(const FObjectInitializer& ObjectInitializer);

public:
	virtual void Enter() override;
	virtual void Tick() override;
	virtual void Exit()	override;
};
