#pragma once

#include "CoreMinimal.h"
#include "UI/System/ResonanceUIStructs.h"
#include "RMessage.generated.h"

UENUM()
enum class ERMessageType
{
	StartGame,
	ShowUI,
	EnterRegion,
	InitStat,
	UpdateStat,
	UpdateEmotion,
};

USTRUCT()
struct FRMessage
{
	GENERATED_BODY()
public:
	FRMessage() { }
	FRMessage(ERMessageType InType) : MessageType(InType) { }

	ERMessageType MessageType;
};

// �ӽ� �뵵
USTRUCT()
struct FRShowUIMessage : public FRMessage
{

	GENERATED_BODY()
public:
	FRShowUIMessage()
		: FRMessage(ERMessageType::ShowUI)
	{}

	FRShowUIMessage(FString InText)
		: FRMessage(ERMessageType::ShowUI)
		, TextValue(InText)
	{ }

	UPROPERTY(Transient)
	FString TextValue;
};

USTRUCT()
struct FRNPCEnterRegion : public FRMessage
{
	GENERATED_BODY()
public:
	FRNPCEnterRegion()
		: FRMessage(ERMessageType::EnterRegion)
	{}

	FRNPCEnterRegion(FName NPCID, FName RegionID, float DurationTime)
		: FRMessage(ERMessageType::EnterRegion)
		, NPCID(NPCID)
		, RegionID(RegionID)
		, HangAroundTime(DurationTime)
	{ }

	// NPC ID
	UPROPERTY(Transient)
	FName NPCID;
	
	// 지역 ID
	UPROPERTY(Transient)
	FName RegionID;
	
	// 머무는 시간 
	UPROPERTY(Transient)
	float HangAroundTime;
};

USTRUCT()
struct FRInitStat : public FRMessage
{
	GENERATED_BODY()
public:
	FRInitStat()
		: FRMessage(ERMessageType::InitStat)
	{}

	FRInitStat(const TArray<FRUIStatInfo>& StatInfos) :
		FRMessage(ERMessageType::InitStat),
		StatInfos(StatInfos)
	{
	}
	UPROPERTY(Transient)
	FName NPCID;
	
	UPROPERTY(Transient)
	TArray<FRUIStatInfo> StatInfos;
};

USTRUCT()
struct FRUpdateStat : public FRMessage
{
	GENERATED_BODY()
public:
	FRUpdateStat()
		: FRMessage(ERMessageType::UpdateStat)
	{}

	FRUpdateStat(ERStatType StatType, float NewValue) :
		FRMessage(ERMessageType::UpdateStat),
		StatType(StatType),
		NewValue(NewValue)
	{
	}
	
	UPROPERTY(Transient)
	FName NPCID;
	
	UPROPERTY(Transient)
	ERStatType StatType;
	
	UPROPERTY(Transient)
	float NewValue;
};

USTRUCT()
struct FRUpdateEmotion : public FRMessage
{
	GENERATED_BODY()
public:
	FRUpdateEmotion()
		: FRMessage(ERMessageType::UpdateEmotion)
	{}

	FRUpdateEmotion(EREmotionState EmotionType) :
		FRMessage(ERMessageType::UpdateEmotion),
		EmotionState(EmotionType)
	{
	}
	
	UPROPERTY(Transient)
	EREmotionState EmotionState;
};