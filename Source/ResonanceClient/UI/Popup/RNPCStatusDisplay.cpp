#include "UI/Popup/RNPCStatusDisplay.h"

#include "Components/TextBlock.h"
#include "System/REventManager.h"
#include "ResonanceCore/Data/RCommonEnums.h"
#include "Data/ResonanceMacro.h"
#include "Data/RMessage.h"

URNPCStatusDisplay::URNPCStatusDisplay(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	StatText = TEXT("HP: {0}\n Stamina: {1}\n Mood: {2}\n Emotion: {3}\n");
}

void URNPCStatusDisplay::OnMessage(ERMessageType Type, FRMessage* Message)
{
	switch (Type)
	{
	case ERMessageType::InitStat:
		{
			FRInitStat* InitStatValue = static_cast<FRInitStat*>(Message);
			if (nullptr != InitStatValue)
			{
				InitStat(InitStatValue->StatInfos);
			}
			break;
		}
	case ERMessageType::UpdateStat:
		{
			FRUpdateStat* UpdateStatValue = static_cast<FRUpdateStat*>(Message);
			if (nullptr != UpdateStatValue)
			{
				UpdateStat(UpdateStatValue->StatType, UpdateStatValue->NewValue);
			}
			break;
		}
	case ERMessageType::UpdateEmotion:
		{
			FRUpdateEmotion* UpdateEmotionValue = static_cast<FRUpdateEmotion*>(Message);
			if (nullptr != UpdateEmotionValue)
			{
				UpdateEmotion(UpdateEmotionValue->EmotionState);
			}
		}
		break;
	}
}

void URNPCStatusDisplay::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	REVENT_MESSAGE_ADD(this, this);
}

void URNPCStatusDisplay::NativeDestruct()
{
	Super::NativeDestruct();

	REVENT_MESSAGE_REMOVE(this, this);
}

void URNPCStatusDisplay::InitStat(const TArray<FRStatInfo>& StatInfos)
{
	for (const FRStatInfo& StatInfo : StatInfos)
	{
		UpdateStat(StatInfo.StatType, StatInfo.Value);
	}
}

void URNPCStatusDisplay::UpdateStat(ERStatType StatType, int32 NewValue)
{
	FString Key = FString::FromInt(static_cast<uint8>(StatType));

	Args.Add(Key, FStringFormatArg(NewValue));

	if (IsValid(TextBlock_Stat))
	{
		FString Result = FString::Format(*StatText, Args);
		TextBlock_Stat->SetText(FText::FromString(Result));
	}
}

void URNPCStatusDisplay::UpdateEmotion(EREmotionState EmotionState)
{
	Args.Add(TEXT("3"), UEnum::GetValueAsString(EmotionState));

	if (IsValid(TextBlock_Stat))
	{
		FString Result = FString::Format(*StatText, Args);
		TextBlock_Stat->SetText(FText::FromString(Result));
	}
}
