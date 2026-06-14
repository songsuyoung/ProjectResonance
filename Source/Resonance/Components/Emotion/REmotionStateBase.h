#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ResonanceStructs.h"
#include "REmotionStateBase.generated.h"

/*
 * EmotionStateBase는 각종 감정들이 상속받는 기본 클래스
 * 즉, 팩토리 메소드 패턴의 기본이 되어지는 클래스
 */

enum class EREmotionState : uint8;

UCLASS(Abstract)
class RESONANCE_API UREmotionStateBase : public UObject
{
	GENERATED_BODY()

public:	
	UREmotionStateBase(const FObjectInitializer& ObjectInitializer);
	
	bool CanTick() { return bCanTick; }
	void Init();
	
public:
	
	// Stat으로 부터 값이 들어 와야 함.
	bool CanExecute(ERStatType StatType, float MaxValue, float Value);
	int32 GetPriority() { return static_cast<int32>(EmotionState); }
	
public:
	virtual void Enter() { }
	virtual void Tick() { }
	virtual void Exit() { }
	
protected:
	float GetConditionValue(float MaxValue, const FREmotionCondition& Condition);
	bool CheckCondition(ERArithmeticOperation OPType, float ConditionValue, float CurrentValue);
protected:
	
	UPROPERTY(EditAnywhere, Category = "Emotion|Settings")
	FREmotionInfo EmotionConditionInfo;
	
protected:
	
	UPROPERTY(Transient)
	uint8 bCanTick : 1;
	
	// 우선 순위를 담당
	UPROPERTY(Transient)
	EREmotionState EmotionState;
	
	UPROPERTY(Transient)
	TMap<ERStatType, bool> ConditionResults;
	
};
