#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/Emotion/REmotionStateBase.h"
#include "REmotionCalm.generated.h"

UCLASS(Blueprintable)
class RESONANCE_API UREmotionCalm : public UREmotionStateBase
{
	GENERATED_BODY()

public:	
	UREmotionCalm(const FObjectInitializer& ObjectInitializer);
	// 항상 기본이 Calm
	virtual bool CanExecute(ERStatType StatType, float MaxValue, float Value) { return true; }
public:
	virtual void Enter() override;
	virtual void Tick() override;
	virtual void Exit()	override;
};
