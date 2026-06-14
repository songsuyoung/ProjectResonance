#include "Components/Emotion/REmotionComponent.h"

// UE

// 
#include "REmotionStateBase.h"
#include "Character/RBaseCharacter.h"
#include "Components/Stat/RBaseStatComponent.h"

UREmotionComponent::UREmotionComponent()
{

}

void UREmotionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ARBaseCharacter* BaseCharacter = Cast<ARBaseCharacter>(GetOwner());
	
	if (IsValid(BaseCharacter))
	{
		StatComponent = BaseCharacter->GetBaseStatComponent();
		
		if (StatComponent.IsValid())
		{
			StatComponent->OnStatChanged.AddUObject(this, &ThisClass::HandleStatChanged);
		}
	}
}

/*
 * 어떻게 기획하는게 좋을까?
 * StatType,Value 값이 변경되어지면, CanCondition에 전달되어 하나라도 반드시 찾게된다.
 * 가장 기본모드인 Calm이 있기 때문임.
 * 만약, Calm을 또 뽑더라도 무언가 변화가 있냐? => 없으면 '<'로 비교가 맞다. (교체 안되도록)
 * 
 * 그러면, 항상 TArray에 0개가 유지되게 할 것인가.. 아니면, StatType에 따라 Angry가 빠졌다. 
 * 근데, 내 생각에는 해당하는 감정을 다 넣고, StatType이 false 일 경우에 해당 스탯 Emotion을 빼고
 * 넣고를 하는게 자연스럽게 감정이 변화되지 않을까 싶다.
 * 
 * 그리고, Emotion 안에는 Priority값에 맞춰 Index를 넣는게 좋지않을까? (관리가 쉬움.)
 * 그런데, Editor상에서 항상 Priority값으로 동작한다는 원리는 없음.
 * 
 * 그리고, Index == HeapTop(저장된 Index) 같음. 
 * 
 * 
 * 실제 활성화된 감정만 Enter/ 활성화되지 않은 감정은 Pop만 실행.
 * 마찬가지로 실제 활성화된 감정만 Exit를 하도록 함.
 */

// StatChanged -> 각 Emotion의 조건 체크 -> 활성 가능한 것들 중 Priority 최고값 -> Enter/Exit
void UREmotionComponent::HandleStatChanged(ERStatType StatType, float MaxValue, float CurrentValue)
{
	// 항상 제거 아래에서 추가하기 때문임. bCanExecute가 true일 때, 
	ActiveEmotionIndexes.Empty();
	// 처음 초반에는 CurrentEmotionKey가 없을 수 있음.
	// 순서대로 반복문 돌기 위해서는, TMap이 아닌, ProrityKey순으로 돌려주어야함.
	for (int32 Index = 0; Index < SortedEmotionKeys.Num(); Index++)
	{
		if (SortedEmotionKeys[Index] >= CurrentEmotionKey)
		{
			//이미 들어가있는 경우임.
			break;
		}
		
		TObjectPtr<UREmotionStateBase>* Emotion = Emotions.Find(SortedEmotionKeys[Index]);
		
		if (nullptr != Emotion && true == (*Emotion)->CanExecute(StatType, MaxValue, CurrentValue))
		{
			ActiveEmotionIndexes.Add(SortedEmotionKeys[Index]);
		}
	}
	
	// 새로 Top이 생긴것.
	if (CurrentEmotionKey != ActiveEmotionIndexes.HeapTop())
	{
		TObjectPtr<UREmotionStateBase>* PreEmotion = Emotions.Find(CurrentEmotionKey);
		
		if (nullptr != PreEmotion)
		{
			(*PreEmotion)->Exit();	
		}
		
		CurrentEmotionKey = ActiveEmotionIndexes.HeapTop();
		
		TObjectPtr<UREmotionStateBase>* CurrentEmotion = Emotions.Find(CurrentEmotionKey);
		
		if (nullptr != CurrentEmotion)
		{
			(*CurrentEmotion)->Enter();
		}
	}
}

float UREmotionComponent::GetStayDuration()
{
	// 추후에 현재 Emotion이 무엇인가에 따라서 머무는 시간이 달라짐.	
	
	//0~1 사이의 값을 가짐 
	float DurationTime = FMath::FRand() * FMath::RandRange(10.f, 50.f);
	
	return DurationTime;
}
