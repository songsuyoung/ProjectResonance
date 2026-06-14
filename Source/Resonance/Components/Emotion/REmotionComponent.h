#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "REmotionComponent.generated.h"

/*
 * 목표 : EmotionStateMachine 을 관리하는 EmotionComponent
 * StatComponent를 약한 포인터로 가지고 있으며, 스탯으로 부터 Emotion 전환을 진행함.
 * Emotion에 의해 관리되어지는 모든 것을 가지고 있는 클래스
 */
class URBaseStatComponent;
class UREmotionStateBase;

enum class ERStatType : uint8;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RESONANCE_API UREmotionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UREmotionComponent();

	float GetStayDuration();
protected:

	virtual void BeginPlay() override;
	void HandleStatChanged(ERStatType StatType, float MaxValue, float CurrentValue);
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotion|Settings")
	TArray<TSubclassOf<UREmotionStateBase>> StateClass;
	
protected:
	
	UPROPERTY(Transient)
	TWeakObjectPtr<URBaseStatComponent> StatComponent;
	
	// <Priority, EmotionState>
	UPROPERTY(Transient)
	TMap<int32, TObjectPtr<UREmotionStateBase>> Emotions;
	
	UPROPERTY(Transient)
	TArray<int32> SortedEmotionKeys;
	
	// Emotions 의 Priority를 담고 있는다.
	// 내림차순 정렬 0번이 가장 우선순위가 높음.
	UPROPERTY(Transient)
	TArray<int32> ActiveEmotionIndexes;
	
	// 현재 EmotionKey 
	// Enter된 Emotion값의 Priority를 저장하여, Exit를 쉽게 한다.
	UPROPERTY(Transient)
	int32 CurrentEmotionKey;
	
};
