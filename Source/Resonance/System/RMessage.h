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

// 임시 용도
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