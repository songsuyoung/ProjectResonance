#include "Components/Emotion/REmotionStateBase.h"

// UE
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// 
#include "Character/RBaseCharacter.h"
#include "Data/ResonanceEnums.h"
#include "Components/Stat/RBaseStatComponent.h"

UREmotionStateBase::UREmotionStateBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bCanTick(false)
	, EmotionState(EREmotionState::Calm)
{
	ConditionResults.Empty();
}

void UREmotionStateBase::Init(ACharacter* InOwner)
{
	// 초기화를 진행.
	for (const auto& Condition :EmotionConditionInfo.Conditions)
	{
		ConditionResults.Add({Condition.StatType, false});
	}
	ARBaseCharacter* BaseCharacter = Cast<ARBaseCharacter>(InOwner);
	
	if (IsValid(BaseCharacter))
	{
		CharacterMovementComponent = BaseCharacter->GetCharacterMovement();
		BaseWalkSpeed = CharacterMovementComponent->MaxWalkSpeed;
		
		StatComponent = BaseCharacter->GetBaseStatComponent();
	}
	
}

float UREmotionStateBase::GetConditionValue(float MaxValue, const FREmotionCondition& Condition)
{
	switch (Condition.ConditionValueType)
	{
	case ERConditionValueType::Percentage:
		return MaxValue * Condition.Value;
	case ERConditionValueType::Value:
		return Condition.Value;
	}
	
	return -1;
}

bool UREmotionStateBase::CheckCondition(ERArithmeticOperation OPType, float ConditionValue, float CurrentValue)
{
	switch (OPType)
	{
	case ERArithmeticOperation::Greater:
		return ConditionValue < CurrentValue;
	case ERArithmeticOperation::GreaterOrEqual:
		return ConditionValue <= CurrentValue;
	case ERArithmeticOperation::Less:
		return ConditionValue > CurrentValue;
	case ERArithmeticOperation::LessOrEqual:
		return ConditionValue >= CurrentValue;
	case ERArithmeticOperation::Equal:
		return ConditionValue == CurrentValue;
	case ERArithmeticOperation::NotEqual:
		return ConditionValue != CurrentValue;
	}
	
	return false;
}

bool UREmotionStateBase::CanExecute(ERStatType StatType, float MaxVale, float Value)
{
	bool* bResult = ConditionResults.Find(StatType);
	
	if (nullptr == bResult)
	{
		return false;
	}
	
	for (const auto& Condition : EmotionConditionInfo.Conditions)
	{
		if (Condition.StatType == StatType)
		{
			float ConditionValue = GetConditionValue(MaxVale, Condition.Condition);
			if (-1 == ConditionValue)
			{
				return false;
			}
			
			*bResult = CheckCondition(Condition.Operator, ConditionValue, Value);
			break;
		}
	}
	
	// 모든 ConditionResults가 true인가를 확인해야함.
	for (const auto& Condition : ConditionResults)
	{
		if (false == Condition.Value)
		{
			return false;
		}
	}
	
	return true;
}

