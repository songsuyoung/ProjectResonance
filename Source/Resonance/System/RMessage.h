#pragma once

#include "CoreMinimal.h"
#include "Data/ResonanceEnums.h"
#include "RMessage.generated.h"

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