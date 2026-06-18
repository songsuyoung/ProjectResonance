#include "Components/Emotion/REmotionComponent.h"

// UE

// 
#include "REmotionStateBase.h"
#include "Character/RBaseCharacter.h"
#include "Character/RNPCCharacter.h"
#include "Components/Stat/RBaseStatComponent.h"
#include "Data/ResonanceMacro.h"
#include "Data/RLocationPreferenceTable.h"
#include "System/RDataManager.h"
#include "Data/ResonanceStructs.h"
#include "Data/RMessage.h"
#include "System/REventManager.h"

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
	
	for (const auto& Class : StateClass)
	{
		UREmotionStateBase* EmotionState = NewObject<UREmotionStateBase>(this, Class);
		
		if (IsValid(EmotionState))
		{
			EmotionState->Init(BaseCharacter);
			uint8 Priority = EmotionState->GetPriority();
			Emotions.Add({Priority, EmotionState});
			SortedEmotionKeys.Add(Priority);
		}
	}
	
	SortedEmotionKeys.Sort();
	CurrentEmotionKey = static_cast<uint8>(EREmotionState::Max); //가장 마지막값으로, 아무것도 없음을 표현
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
	ActiveEmotionIndexes.Empty();
	// 처음 초반에는 CurrentEmotionKey가 없을 수 있음.
	// 순서대로 반복문 돌기 위해서는, TMap이 아닌, ProrityKey순으로 돌려주어야함.
	for (int32 Index = 0; Index < SortedEmotionKeys.Num(); Index++)
	{
		TObjectPtr<UREmotionStateBase>* Emotion = Emotions.Find(SortedEmotionKeys[Index]);
		
		if (nullptr != Emotion)
		{
			if (true == (*Emotion)->CanExecute(StatType, MaxValue, CurrentValue))
			{
				ActiveEmotionIndexes.Add(SortedEmotionKeys[Index]);
			}
		}
	}
	
	if (ActiveEmotionIndexes.IsEmpty())
	{
		// 활성화된 Emotion이 없음을 의미함.
		return;
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
			
			FRUpdateEmotion UpdateEmotion((*CurrentEmotion)->GetEmotionType());
			REVENT_MESSAGE_NOTIFY_MSG(this, ERMessageType::UpdateEmotion, UpdateEmotion);
		}
	}
}

float UREmotionComponent::GetStayDuration(FName LocationID)
{
	ARNPCCharacter* NPCCharacter = Cast<ARNPCCharacter>(GetOwner());
	
	if (false == IsValid(NPCCharacter))
	{
		return 0.0f;
	}
	
	URDataManager* DataManager = URDataManager::Get(this);
	check(DataManager);
	
	FRLocationPreferenceTable* PreferenceTable = DataManager->GetDataTableRow<FRLocationPreferenceTable>(ERDataTableType::LocationPreferenceDataTable, NPCCharacter->GetPreferenceLocationID());
	
	if (nullptr == PreferenceTable)
	{
		return 0.0f;
	}
	
	float Stamina = StatComponent->GetCurrentStatValue(ERStatType::Stamina) / FMath::Max(StatComponent->GetMaxStatValue(ERStatType::Stamina), 1.0f);
	float StaminaMult = 1.0 + (1.0f - Stamina); //역비례해서 스테미나가 적을 수록 더 오래 머물도록 함.
	
	// TODO: 크래시 발생, Emotion 시스템에서 선호도를 조사하는 코드 좀 더 생각해보기.
	EREmotionState EmotionState = Emotions[CurrentEmotionKey]->GetEmotionType();
	float EmotionMult = Emotions[CurrentEmotionKey]->GetHangAroundMultiplier();
	
	float* Found = PreferenceTable->Preferences[EmotionState].Preferences.Find(LocationID);
	float LocationPref = 1.0f;
	if (Found != nullptr)
	{
		LocationPref = *Found;
	}
	
	// 추후에 현재 Emotion이 무엇인가에 따라서 머무는 시간이 달라짐.	
	float TotalDuration = NPCCharacter->GetBaseDuration() * EmotionMult * LocationPref * StaminaMult;
	//0~1 사이의 값을 가짐 
	float DurationTime = FMath::Clamp(TotalDuration, 5.0f, 20.f);
	
	return DurationTime;
}

EREmotionState UREmotionComponent::GetEmotionState()
{
	if (Emotions.Contains(CurrentEmotionKey))
	{
		return Emotions[CurrentEmotionKey]->GetEmotionType();
	}
	
	return EREmotionState::Max;
}
