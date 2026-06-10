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
struct FRNPCReleaseMessage : public FRMessage
{
	GENERATED_BODY()
public:
	FRNPCReleaseMessage()
		: FRMessage(ERMessageType::EnterRegion)
	{}

	FRNPCReleaseMessage(FName NPCID, FName RegionID)
		: FRMessage(ERMessageType::EnterRegion)
		, NPCID(NPCID)
		, RegionID(RegionID)
	{ }

	UPROPERTY(Transient)
	FName NPCID;
	
	UPROPERTY(Transient)
	FName RegionID;
};